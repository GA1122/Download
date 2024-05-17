LRESULT WebView::onTimerEvent(HWND hWnd, UINT, WPARAM wParam, LPARAM, bool& handled)
{
    switch (wParam) {
    case UpdateActiveStateTimer:
        ::KillTimer(hWnd, UpdateActiveStateTimer);
        updateActiveState();
        break;
    }

    handled = true;
    return 0;
}
