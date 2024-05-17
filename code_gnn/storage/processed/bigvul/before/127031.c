void ChromeClientImpl::invalidateContentsAndRootView(const IntRect& updateRect)
{
    if (updateRect.isEmpty())
        return;
    m_webView->invalidateRect(updateRect);
}
