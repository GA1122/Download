LRESULT WebView::onHorizontalScroll(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, bool& handled)
{
    ScrollDirection direction;
    ScrollGranularity granularity;
    switch (LOWORD(wParam)) {
    case SB_LINELEFT:
        granularity = ScrollByLine;
        direction = ScrollLeft;
        break;
    case SB_LINERIGHT:
        granularity = ScrollByLine;
        direction = ScrollRight;
        break;
    case SB_PAGELEFT:
        granularity = ScrollByDocument;
        direction = ScrollLeft;
        break;
    case SB_PAGERIGHT:
        granularity = ScrollByDocument;
        direction = ScrollRight;
        break;
    default:
        handled = false;
        return 0;
    }

    m_page->scrollBy(direction, granularity);

    handled = true;
    return 0;
}
