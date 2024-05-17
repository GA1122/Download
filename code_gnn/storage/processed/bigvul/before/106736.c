LRESULT WebView::onMouseEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, bool& handled)
{
    WebMouseEvent mouseEvent = WebEventFactory::createWebMouseEvent(hWnd, message, wParam, lParam, m_wasActivatedByMouseEvent);
    setWasActivatedByMouseEvent(false);
    
    switch (message) {
    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
        ::SetFocus(m_window);
        ::SetCapture(m_window);
        break; 
    case WM_LBUTTONUP:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP:
        ::ReleaseCapture();
        break;
    case WM_MOUSEMOVE:
        startTrackingMouseLeave();
        break;
    case WM_MOUSELEAVE:
        stopTrackingMouseLeave();
        break;
    case WM_LBUTTONDBLCLK:
    case WM_MBUTTONDBLCLK:
    case WM_RBUTTONDBLCLK:
        break;
    default:
        ASSERT_NOT_REACHED();
    }

    m_page->handleMouseEvent(mouseEvent);

    handled = true;
    return 0;
}
