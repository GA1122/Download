void ChromeClientImpl::invalidateContentsForSlowScroll(const IntRect& updateRect)
{
    invalidateContentsAndRootView(updateRect);
}
