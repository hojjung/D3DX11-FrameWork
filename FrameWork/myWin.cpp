#include "stdafx.h"

C_MYWIN * C_MYWIN::m_pMyWin = nullptr;

const POINT	C_MYWIN::m_sCursorOffset = { 500,500 };

void C_MYWIN::getMousePosition(LPPOINT pt)
{
	GetCursorPos(pt);
	ScreenToClient(m_hWnd, pt);

	
}

void C_MYWIN::setMoveCursorOffset()
{
	SetPhysicalCursorPos(m_sCursorOffset.x, m_sCursorOffset.y);
}

C_MYWIN::C_MYWIN() :
	m_hInstance{},
	m_hWnd{},
	m_pGraphic{},
	m_pCamera{},
	m_arKeyEventFunc{},
	m_listKeyEvent{}
{
}

void C_MYWIN::createMyWin()
{
	if (!m_pMyWin)
		m_pMyWin = new C_MYWIN{};
}

C_MYWIN * C_MYWIN::getMyWin()
{
	return m_pMyWin;
}

void C_MYWIN::deleteMyWin()
{
	if (m_pMyWin)
	{
		delete m_pMyWin;
		m_pMyWin = nullptr;
	}
}

void C_MYWIN::init(HINSTANCE hInstance)
{
	m_hInstance = hInstance;

	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = wndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInstance;
	wcex.hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_FRAMEWORK));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = 0;
	wcex.lpszClassName = L"frameWork";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	RegisterClassExW(&wcex);

	RECT rc = { 0, 0, 1024, 768 };
	DWORD dwWinStyle = WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME;
	AdjustWindowRect(&rc, dwWinStyle, FALSE);

	m_hWnd = CreateWindowW(L"frameWork", L"",
		dwWinStyle,
		CW_USEDEFAULT, 0,
		rc.right - rc.left, rc.bottom - rc.top,
		nullptr, nullptr, m_hInstance, nullptr);

	if (!m_hWnd)
		return;

	initField();

	ShowWindow(m_hWnd, SW_SHOWDEFAULT);
	UpdateWindow(m_hWnd);
}

void C_MYWIN::updateMsg()
{
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			std::list<int>::iterator iter = m_listKeyEvent.begin();
			while (iter != m_listKeyEvent.end())
			{
				if (GetAsyncKeyState(*iter) & 0x8001)
					(this->*m_arKeyEventFunc[*iter]) ();

				iter++;
			}

			POINT sPt = {};
			GetPhysicalCursorPos(&sPt);

			if (m_sCursorOffset.x != sPt.x || m_sCursorOffset.y != sPt.y)
			{
				WCHAR szPrintText[128] = L"";
				swprintf_s(szPrintText, 128, L"before x %d , y %d\n", sPt.x, sPt.y);
				OutputDebugString(szPrintText);

				setMoveCursorOffset();

				float fYaw = (float)(sPt.x - m_sCursorOffset.x) * 0.001f;
				float fPitch = (float)(sPt.y - m_sCursorOffset.y) * 0.001f;
				m_pCamera->rotateYawPitchRoll(fYaw, fPitch, 0.0f);
			}
			m_pCamera->update();
			m_pGraphic->Render(m_pCamera->getMatrixView());
		}
	}
}

void C_MYWIN::release()
{
	m_pGraphic->CleanupDevice();
	delete m_pGraphic;
	m_pGraphic = nullptr;


}

LRESULT CALLBACK C_MYWIN::wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return m_pMyWin->myProc(hWnd, message, wParam, lParam);
}

LRESULT C_MYWIN::myProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	static bool bShowCursor = false;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		return S_OK;

	case WM_DESTROY:
		PostQuitMessage(0);
		return S_OK;

	case WM_LBUTTONDOWN:

		ShowCursor(bShowCursor);
		bShowCursor = !bShowCursor;
		return S_OK;

	case WM_MOUSEMOVE:

		return S_OK;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

bool C_MYWIN::initField()
{

	m_pGraphic = new C_GRAPHIC{};

	if (FAILED(m_pGraphic->InitDevice(m_hWnd)))
	{
		m_pGraphic->CleanupDevice();
		return false;
	}

	m_pCamera = new C_CAMERA{};
	m_pCamera->setPosition(0.0f, 4.0f, -10.0f);
	m_pCamera->rotateYawPitchRoll(0.0f, 0.4f, 0.0f);

	initKeyEvent();
	setMoveCursorOffset();

	return true;
}

void C_MYWIN::initKeyEvent()
{
	addEvent('W', &C_MYWIN::eventFoward);
	addEvent('A', &C_MYWIN::eventLeft);
	addEvent('D', &C_MYWIN::eventRight);
	addEvent('S', &C_MYWIN::eventBack);
	addEvent(VK_ESCAPE, &C_MYWIN::eventEsc);
}
void C_MYWIN::addEvent(int vKey, void(C_MYWIN::* pEvent)())
{
	m_arKeyEventFunc[vKey] = pEvent;
	m_listKeyEvent.push_back(vKey);
}

void C_MYWIN::eventLeft()
{
	m_pCamera->moveL();

}

void C_MYWIN::eventRight()
{
	m_pCamera->moveR();

}

void C_MYWIN::eventFoward()
{
	m_pCamera->moveF();
}

void C_MYWIN::eventBack()
{
	m_pCamera->moveB();

}

void C_MYWIN::eventEsc()
{
	DestroyWindow(m_hWnd);
}
