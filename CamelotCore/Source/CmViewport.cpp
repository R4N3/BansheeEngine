/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2011 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#include "CmViewport.h"

#include "CmException.h"
#include "CmRenderTarget.h"
#include "CmMath.h"
#include "CmRenderSystem.h"

namespace CamelotEngine {

	//---------------------------------------------------------------------
	Viewport::Viewport()
		:mTarget(nullptr)
		, mRelLeft(0)
		, mRelTop(0)
		, mRelWidth(0)
		, mRelHeight(0)
		// Actual dimensions will update later
		, mZOrder(0)
	{
		// Calculate actual dimensions
		updateDimensions();
	}
    //---------------------------------------------------------------------
    Viewport::Viewport(RenderTargetPtr target, float left, float top, float width, float height, int ZOrder)
         :mTarget(target)
        , mRelLeft(left)
        , mRelTop(top)
        , mRelWidth(width)
        , mRelHeight(height)
        // Actual dimensions will update later
        , mZOrder(ZOrder)
    {
        // Calculate actual dimensions
        updateDimensions();
    }
    //---------------------------------------------------------------------
    Viewport::~Viewport()
    {

    }
    //---------------------------------------------------------------------
    void Viewport::updateDimensions(void)
    {
		if(mTarget != nullptr)
		{
			float height = (float) mTarget->getHeight();
			float width = (float) mTarget->getWidth();

			mActLeft = (int) (mRelLeft * width);
			mActTop = (int) (mRelTop * height);
			mActWidth = (int) (mRelWidth * width);
			mActHeight = (int) (mRelHeight * height);
		}
    }
	//---------------------------------------------------------------------
	int Viewport::getZOrder(void) const
	{
		return mZOrder;
	}
	//---------------------------------------------------------------------
    RenderTargetPtr Viewport::getTarget(void) const
    {
        return mTarget;
    }
    //---------------------------------------------------------------------
    float Viewport::getLeft(void) const
    {
        return mRelLeft;
    }
    //---------------------------------------------------------------------
    float Viewport::getTop(void) const
    {
        return mRelTop;
    }
    //---------------------------------------------------------------------
    float Viewport::getWidth(void) const
    {
        return mRelWidth;
    }
    //---------------------------------------------------------------------
    float Viewport::getHeight(void) const
    {
        return mRelHeight;
    }
    //---------------------------------------------------------------------
    int Viewport::getActualLeft(void) const
    {
        return mActLeft;
    }
    //---------------------------------------------------------------------
    int Viewport::getActualTop(void) const
    {
        return mActTop;
    }
    //---------------------------------------------------------------------
    int Viewport::getActualWidth(void) const
    {
        return mActWidth;
    }
    //---------------------------------------------------------------------
    int Viewport::getActualHeight(void) const
    {
        return mActHeight;
    }
    //---------------------------------------------------------------------
    void Viewport::setDimensions(float left, float top, float width, float height)
    {
        mRelLeft = left;
        mRelTop = top;
        mRelWidth = width;
        mRelHeight = height;
        updateDimensions();
    }
    //---------------------------------------------------------------------
    void Viewport::getActualDimensions(int &left, int&top, int &width, int &height) const
    {
        left = mActLeft;
        top = mActTop;
        width = mActWidth;
        height = mActHeight;

    }
}