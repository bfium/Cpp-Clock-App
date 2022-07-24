#pragma once
/*
	Copyright (C) 2022  Barth.Feudong
	Author can be contacted here: <https://github.com/mrSchaffman/Cpp-Clock-App>

	This file is part of the Clock project. using the Win32 API and The COM

	Clock is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Clock is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

#include "BaseWindow.h"
#include<d2d1.h>
#pragma comment(lib,"d2d1")

template <class T>
void SafeRelease(T**ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}

class MainWindow : public BaseWindow<MainWindow>
{
public:
	MainWindow() :pFactory(NULL), pRenderTarget(NULL), pBrush(NULL) {}

	PCWSTR  ClassName() const { return L"Sample Window Class"; }
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	// COM interfaces...
	ID2D1Factory * pFactory;
	ID2D1HwndRenderTarget * pRenderTarget;
	ID2D1SolidColorBrush * pBrush;			
	D2D1_ELLIPSE			ellipse;		

	// Recalculate Drawing layout when the size of the Window changes.
	void CalculateLayout();

	// Create the Resource needed for Drawing
	HRESULT CreateGraphicsResource();
	void DiscardGraphicsResource();

	// Draw
	void OnPaint();
	void Resize();
};

