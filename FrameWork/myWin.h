#pragma once

#include <windows.h>
#include <list>

#include "graphic.h"
#include "camera.h"

class C_MYWIN
{
private:
	static C_MYWIN * m_pMyWin;

	HINSTANCE		m_hInstance;
	HWND			m_hWnd;

	C_GRAPHIC*      m_pGraphic;
	C_CAMERA*		m_pCamera;

	void			(C_MYWIN::*m_arKeyEventFunc[256]) ();
	std::list<int>  m_listKeyEvent;


	static const POINT	m_sCursorOffset;

private: 
	void			addEvent(int vKey, void (C_MYWIN::* pEvent) ());
	void			getMousePosition(LPPOINT pt);
	void			setMoveCursorOffset();

private:
	C_MYWIN();

	static LRESULT CALLBACK wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK myProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	bool initField();
	void initKeyEvent();
	void eventLeft();
	void eventRight();
	void eventFoward();
	void eventBack();
	void eventEsc();

public:
	static void createMyWin();
	static C_MYWIN * getMyWin();
	static void deleteMyWin();

	void init(HINSTANCE hInstance);
	void updateMsg();
	void release();
};