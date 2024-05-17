LRESULT WebView::onWheelEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, bool& handled)
{
    WebWheelEvent wheelEvent = WebEventFactory::createWebWheelEvent(hWnd, message, wParam, lParam);
    if (wheelEvent.controlKey()) {
        handled = false;
        return 0;
    }

    m_page->handleWheelEvent(wheelEvent);

    handled = true;
    return 0;
}
