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
#ifndef UNICODE
#define UNICODE
#endif // UNICODE

#include<Windows.h>
#include"MainWindow.h"
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPInstance, PWSTR pCmdLine, int nCmdShow)
{
	MainWindow win;

	if (!win.Create(L"Clock App", WS_OVERLAPPEDWINDOW, WS_EX_APPWINDOW, 400, 400, 400, 400))
	{
		return 0;
	}

	ShowWindow(win.Window(), nCmdShow);

	// enter the message loop: The application needs a loop to retrieve the messages and dispatch them to the correct windows.
	MSG message{};
	while (GetMessage(&message, NULL, 0, 0) > 0)
	{
		// exemple of message: #define WM_LBUTTONDOWN    0x0201

		TranslateMessage(&message);
		DispatchMessage(&message);
	}


	return 0;
}
