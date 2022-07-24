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

#include "MainWindow.h"
LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory)))		// initialize the Factory
			return -1;	// The Creation of the Window failed.
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;


	case WM_SIZE:
		Resize();
		return 0;

	case WM_PAINT:
		OnPaint();
		return 0;

	default:
		return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
	}
	return TRUE;
}

void MainWindow::CalculateLayout()
{
	// Get the Size of the Render Target
	D2D1_SIZE_F size = pRenderTarget->GetSize(); // returns the size of the render target in DIPs (not pixels), which is the appropriate unit for calculating layout.

	// get the Center of the Render Target
	const float x = size.width / 2;
	const float y = size.height / 2;

	// the radius of the ellipse according to the width and the heigh
	const float radius = min(x, y);

	// create the Ellipse
	ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);
}

HRESULT MainWindow::CreateGraphicsResource()
{
	HRESULT hr = S_OK;
	if (pRenderTarget == NULL)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

		hr = pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(m_hwnd, size), &pRenderTarget);

		if (SUCCEEDED(hr))
		{
			const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 0.0f, 1.0f, 0.1f);
			/*
				// Initialize a magenta color.
				D2D1_COLOR_F clr;
				clr.r = 1;
				clr.g = 0;
				clr.b = 1;
				clr.a = 1;  // Opaque.
			*/
			hr = pRenderTarget->CreateSolidColorBrush(color, &pBrush);

			if (SUCCEEDED(hr))
			{
				CalculateLayout();
			}
		}

	}
	return hr;
}

void MainWindow::DiscardGraphicsResource()
{
	SafeRelease(&pRenderTarget);
	SafeRelease(&pBrush);
}

void MainWindow::OnPaint()
{
	HRESULT hr = CreateGraphicsResource();

	if (SUCCEEDED(hr))
	{
		PAINTSTRUCT ps;

		BeginPaint(m_hwnd, &ps);

		pRenderTarget->BeginDraw();
		pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::LightSkyBlue));		// Draw Command 1
		hr = pRenderTarget->EndDraw();		// check if the Draw was successful

		// Direct2D signals a lost device by returning the error code D2DERR_RECREATE_TARGET from the EndDraw method. 
		// If you receive this error code, you must re-create the render target and all device-dependent resources.
		if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
		{
			DiscardGraphicsResource();
		}

		EndPaint(m_hwnd, &ps);
	}
}

void MainWindow::Resize()
{

	if (pRenderTarget != NULL)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

		pRenderTarget->Resize(size);
		CalculateLayout();



		InvalidateRect(m_hwnd, NULL, FALSE);
	}
}
