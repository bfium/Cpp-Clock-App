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


void MainWindow::DrawClockHand(float fHandLength, float fAngle, float fStrockeWidth)
{
	pRenderTarget->SetTransform(
		D2D1::Matrix3x2F::Rotation(fAngle, ellipse.point)
	);

	// endPoint defines one end of the hand.
	D2D_POINT_2F endPoint = D2D1::Point2F(
		ellipse.point.x,
		ellipse.point.y - (ellipse.radiusY * fHandLength)
	);

	// Draw a line from the center of the ellipse to endPoint.
	pRenderTarget->DrawLine(
		ellipse.point, 
		endPoint, 
		pBrush, 
		fStrockeWidth
	);
}

void MainWindow::RenderClock()
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


	pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));

	pRenderTarget->FillEllipse(ellipse, pBrush);

	// Draw hands
	SYSTEMTIME time;
	GetLocalTime(&time);

	// 60 minutes = 30 degrees, 1 minute = 0.5 degree
	const float fHourAngle = (360.0f / 12) * (time.wHour) + (time.wMinute * 0.5f);
	const float fMinuteAngle = (360.0f / 60) * (time.wMinute);

	DrawClockHand(0.6f, fHourAngle, 6);
	DrawClockHand(0.85f, fMinuteAngle, 4);

	// Restore the identity transformation.
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
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
			const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 0.0f, 0.5f);
			const D2D1_COLOR_F handColor = D2D1::ColorF(1.0f, 0.0f, 0.0f, 0.5f);
			/*
				// Initialize a magenta color.
				D2D1_COLOR_F clr;
				clr.r = 1;
				clr.g = 0;
				clr.b = 1;
				clr.a = 1;  // Opaque.
			*/
			hr = pRenderTarget->CreateSolidColorBrush(color, &pBrush);
			hr = pRenderTarget->CreateSolidColorBrush(handColor, &pBrushHand);

			if (FAILED(hr))
			{
				DiscardGraphicsResource();
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
		pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));		// Draw Command 1
		RenderClock();
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
		RenderClock();



		InvalidateRect(m_hwnd, NULL, FALSE);
	}
}
