LRESULT WebView::onPaintEvent(HWND hWnd, UINT message, WPARAM, LPARAM, bool& handled)
{
    PAINTSTRUCT paintStruct;
    HDC hdc = ::BeginPaint(m_window, &paintStruct);

    if (WebPageProxy::debugPaintFlags() & kWKDebugFlashViewUpdates) {
        static HBRUSH brush = createBrush(WebPageProxy::viewUpdatesFlashColor().rgb()).leakPtr();
        IntRect rect = paintStruct.rcPaint;
        flashRects(hdc, &rect, 1, brush);
    }

    paint(hdc, paintStruct.rcPaint);

    ::EndPaint(m_window, &paintStruct);

    handled = true;
    return 0;
}
