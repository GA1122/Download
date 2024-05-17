WebCore::IntSize WebView::viewSize()
{
    RECT clientRect;
    GetClientRect(m_window, &clientRect);

    return IntRect(clientRect).size();
}
