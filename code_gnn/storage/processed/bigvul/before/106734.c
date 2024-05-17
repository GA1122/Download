LRESULT WebView::onKeyEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, bool& handled)
{
    m_page->handleKeyboardEvent(NativeWebKeyboardEvent(hWnd, message, wParam, lParam));

    handled = true;
    return 0;
}
