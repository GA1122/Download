LRESULT WebView::wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT lResult = 0;
    bool handled = true;

    switch (message) {
    case WM_CLOSE:
        m_page->tryClose();
        break;
    case WM_DESTROY:
        m_isBeingDestroyed = true;
        close();
        break;
    case WM_ERASEBKGND:
        lResult = 1;
        break;
    case WM_PAINT:
        lResult = onPaintEvent(hWnd, message, wParam, lParam, handled);
        break;
    case WM_PRINTCLIENT:
        lResult = onPrintClientEvent(hWnd, message, wParam, lParam, handled);
        break;
    case WM_MOUSEACTIVATE:
        setWasActivatedByMouseEvent(true);
        handled = false;
        break;
    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_LBUTTONDBLCLK:
    case WM_MBUTTONDBLCLK:
    case WM_RBUTTONDBLCLK:
    case WM_LBUTTONUP:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MOUSELEAVE:
        lResult = onMouseEvent(hWnd, message, wParam, lParam, handled);
        break;
    case WM_MOUSEWHEEL:
    case WM_VISTA_MOUSEHWHEEL:
        lResult = onWheelEvent(hWnd, message, wParam, lParam, handled);
        break;
    case WM_HSCROLL:
        lResult = onHorizontalScroll(hWnd, message, wParam, lParam, handled);
        break;
    case WM_VSCROLL:
        lResult = onVerticalScroll(hWnd, message, wParam, lParam, handled);
        break;
    case WM_GESTURENOTIFY:
        lResult = onGestureNotify(hWnd, message, wParam, lParam, handled);
        break;
    case WM_GESTURE:
        lResult = onGesture(hWnd, message, wParam, lParam, handled);
        break;
    case WM_SYSKEYDOWN:
    case WM_KEYDOWN:
    case WM_SYSCHAR:
    case WM_CHAR:
    case WM_SYSKEYUP:
    case WM_KEYUP:
        lResult = onKeyEvent(hWnd, message, wParam, lParam, handled);
        break;
    case WM_SIZE:
        lResult = onSizeEvent(hWnd, message, wParam, lParam, handled);
        break;
    case WM_WINDOWPOSCHANGED:
        lResult = onWindowPositionChangedEvent(hWnd, message, wParam, lParam, handled);
        break;
    case WM_SETFOCUS:
        lResult = onSetFocusEvent(hWnd, message, wParam, lParam, handled);
        break;
    case WM_KILLFOCUS:
        lResult = onKillFocusEvent(hWnd, message, wParam, lParam, handled);
        break;
    case WM_TIMER:
        lResult = onTimerEvent(hWnd, message, wParam, lParam, handled);
        break;
    case WM_SHOWWINDOW:
        lResult = onShowWindowEvent(hWnd, message, wParam, lParam, handled);
        break;
    case WM_SETCURSOR:
        lResult = onSetCursor(hWnd, message, wParam, lParam, handled);
        break;
    case WM_IME_STARTCOMPOSITION:
        handled = onIMEStartComposition();
        break;
    case WM_IME_REQUEST:
        lResult = onIMERequest(wParam, lParam);
        break;
    case WM_IME_COMPOSITION:
        handled = onIMEComposition(lParam);
        break;
    case WM_IME_ENDCOMPOSITION:
        handled = onIMEEndComposition();
        break;
    case WM_IME_SELECT:
        handled = onIMESelect(wParam, lParam);
        break;
    case WM_IME_SETCONTEXT:
        handled = onIMESetContext(wParam, lParam);
        break;
    default:
        handled = false;
        break;
    }

    if (!handled)
        lResult = ::DefWindowProc(hWnd, message, wParam, lParam);

    return lResult;
}
