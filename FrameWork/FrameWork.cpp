// FrameWork.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	C_MYWIN::createMyWin();

	C_MYWIN::getMyWin()->init(hInstance);
	C_MYWIN::getMyWin()->updateMsg();
	C_MYWIN::getMyWin()->release();

	C_MYWIN::deleteMyWin();

    return 0;
}

