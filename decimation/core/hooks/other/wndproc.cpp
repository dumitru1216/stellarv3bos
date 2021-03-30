#include "../../../includes.h"

#include <imgui/imgui_impl_win32.h>
#include "../../../new_gui/m_elements.h"
#include <windowsx.h>

IMGUI_IMPL_API LRESULT  ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI Hooks::WndProc( HWND wnd, uint32_t msg, WPARAM wp, LPARAM lp ) {
	if ( msg == WM_MOUSEMOVE )
		Gui::m_Input.SetMousePos( vec2_t( GET_X_LPARAM( lp ), GET_Y_LPARAM( lp ) ) );

	if ( Gui::m_Details.GetMenuState( ) && msg == WM_MOUSEWHEEL )
		Gui::m_Input.SetMouseWheel( GET_WHEEL_DELTA_WPARAM( wp ) / WHEEL_DELTA );

	return g_winapi.CallWindowProcA( g_hooks.m_old_wndproc, wnd, msg, wp, lp );
}