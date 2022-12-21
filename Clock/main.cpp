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

#include"app.h"

using namespace app;

 int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPInstance, PWSTR pCmdLine, int nCmdShow)
 {
	 HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	 Facade facade;
	 facade.run();

	 return 0;
 }

//int main()
//{
//	using namespace app;
//	Facade facade;
//	facade.run();
//
//	return 0;
//}