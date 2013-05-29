#include "BsTextSprite.h"
#include "BsGUIMaterialManager.h"
#include "CmTextUtility.h"
#include "CmFont.h"
#include "CmVector2.h"

using namespace CamelotFramework;

namespace BansheeEngine
{
	TextSprite::TextSprite()
	{

	}

	void TextSprite::update(const TEXT_SPRITE_DESC& desc)
	{
		std::shared_ptr<TextUtility::TextData> textData = TextUtility::getTextData(desc.text, desc.font, desc.fontSize, desc.width, desc.height, desc.wordWrap);

		if(textData == nullptr)
			return;

		const CM::vector<TextUtility::TextLine>::type& lines = textData->getLines();
		const CM::vector<UINT32>::type& quadsPerPage = textData->getNumQuadsPerPage();

		UINT32 curHeight = 0;
		for(auto& line : lines)
		{
			curHeight += line.getYOffset();
		}

		// Calc vertical alignment offset
		UINT32 vertDiff = std::max(0U, desc.height - curHeight);
		UINT32 vertOffset = 0;
		switch(desc.vertAlign)
		{
		case TVA_Top:
			vertOffset = 0;
			break;
		case TVA_Bottom:
			vertOffset = std::max(0, (INT32)(desc.height - curHeight));
			break;
		case TVA_Center:
			vertOffset = std::max(0, (INT32)(desc.height - curHeight)) / 2;
			break;
		}

		// Actually generate a mesh
		if(mCachedRenderElements.size() < quadsPerPage.size())
			mCachedRenderElements.resize(quadsPerPage.size());

		const CM::vector<HTexture>::type& texturePages = textData->getTexturePages();
		UINT32 texPage = 0;
		for(auto& cachedElem : mCachedRenderElements)
		{
			UINT32 newNumQuads = quadsPerPage[texPage];
			if(newNumQuads != cachedElem.numQuads)
			{
				UINT32 oldVertexCount = cachedElem.numQuads * 4;
				UINT32 oldIndexCount = cachedElem.numQuads * 6;

				if(cachedElem.vertices != nullptr) CM_DELETE_ARRAY(cachedElem.vertices, Vector2, oldVertexCount, ScratchAlloc);
				if(cachedElem.uvs != nullptr) CM_DELETE_ARRAY(cachedElem.uvs, Vector2, oldVertexCount, ScratchAlloc);
				if(cachedElem.indexes != nullptr) CM_DELETE_ARRAY(cachedElem.indexes, UINT32, oldIndexCount, ScratchAlloc);

				cachedElem.vertices = CM_NEW_ARRAY(Vector2, newNumQuads * 4, ScratchAlloc);
				cachedElem.uvs = CM_NEW_ARRAY(Vector2, newNumQuads * 4, ScratchAlloc);
				cachedElem.indexes = CM_NEW_ARRAY(UINT32, newNumQuads * 6, ScratchAlloc);
				cachedElem.numQuads = newNumQuads;
			}

			HMaterial newMaterial = GUIMaterialManager::instance().requestTextMaterial(texturePages[texPage]);
			if(cachedElem.material != nullptr)
				GUIMaterialManager::instance().releaseMaterial(newMaterial);

			cachedElem.material = newMaterial;

			texPage++;
		}

		// Calc horizontal alignment offset and set final line positions
		Int2 offset = getAnchorOffset(desc.anchor, desc.width, desc.height);
		UINT32 numPages = (UINT32)quadsPerPage.size();
		UINT32 curY = 0;
		vector<UINT32>::type faceOffsets(mCachedRenderElements.size(), 0);
		for(size_t i = 0; i < lines.size(); i++)
		{
			UINT32 horzOffset = 0;
			switch(desc.horzAlign)
			{
			case THA_Left:
				horzOffset = 0;
				break;
			case THA_Right:
				horzOffset = std::max(0, (INT32)(desc.width - lines[i].getWidth()));
				break;
			case THA_Center:
				horzOffset = std::max(0, (INT32)(desc.width - lines[i].getWidth())) / 2;
				break;
			}

			Int2 position = offset + Int2(horzOffset, vertOffset + curY);
			curY += lines[i].getYOffset();

			for(size_t j = 0; j < numPages; j++)
			{
				SpriteRenderElement& renderElem = mCachedRenderElements[j];
				UINT32 offset = faceOffsets[j];

				UINT32 writtenQuads = lines[i].fillBuffer((UINT32)j, renderElem.vertices, renderElem.uvs, renderElem.indexes, offset, renderElem.numQuads);
				
				UINT32 numVertices = writtenQuads * 4;
				for(size_t i = 0; i < numVertices; i++)
				{
					renderElem.vertices[offset + i].x += (float)position.x;
					renderElem.vertices[offset + i].y += (float)position.y;
				}

				faceOffsets[j] += writtenQuads;
			}
		}

		if(desc.clipRect.width > 0 && desc.clipRect.height > 0)
		{
			for(auto& renderElem : mCachedRenderElements)
			{
				clipToRect(renderElem.vertices, renderElem.uvs, renderElem.numQuads, desc.clipRect);
			}
		}

		// Apply offset
		for(auto& renderElem : mCachedRenderElements)
		{
			UINT32 numVertices = renderElem.numQuads * 4;
			for(size_t i = 0; i < numVertices; i++)
			{
				renderElem.vertices[i].x += (float)desc.offset.x;
				renderElem.vertices[i].y += (float)desc.offset.y;
			}
		}

		updateBounds();
	}
}