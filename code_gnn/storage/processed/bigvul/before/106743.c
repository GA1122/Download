LRESULT WebView::onVerticalScroll(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, bool& handled)
{
    ScrollDirection direction;
    ScrollGranularity granularity;
    switch (LOWORD(wParam)) {
    case SB_LINEDOWN:
        granularity = ScrollByLine;
        direction = ScrollDown;
        break;
    case SB_LINEUP:
        granularity = ScrollByLine;
        direction = ScrollUp;
        break;
    case SB_PAGEDOWN:
        granularity = ScrollByDocument;
        direction = ScrollDown;
        break;
    case SB_PAGEUP:
        granularity = ScrollByDocument;
        direction = ScrollUp;
        break;
    default:
        handled = false;
        return 0;
    }

    m_page->scrollBy(direction, granularity);

    handled = true;
    return 0;
}
