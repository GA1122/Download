void WebView::initialize()
{
    ::RegisterDragDrop(m_window, this);

    if (shouldInitializeTrackPointHack()) {
        ::CreateWindow(TEXT("SCROLLBAR"), TEXT("FAKETRACKPOINTHSCROLLBAR"), WS_CHILD | WS_VISIBLE | SBS_HORZ, 0, 0, 0, 0, m_window, 0, instanceHandle(), 0);
        ::CreateWindow(TEXT("SCROLLBAR"), TEXT("FAKETRACKPOINTVSCROLLBAR"), WS_CHILD | WS_VISIBLE | SBS_VERT, 0, 0, 0, 0, m_window, 0, instanceHandle(), 0);
    }
}
