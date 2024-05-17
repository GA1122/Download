bool ChromeClientImpl::shouldRubberBandInDirection(WebCore::ScrollDirection direction) const
{
    ASSERT(direction != WebCore::ScrollUp && direction != WebCore::ScrollDown);

    if (!m_webView->client())
        return false;

    if (direction == WebCore::ScrollLeft)
        return !m_webView->client()->historyBackListCount();
    if (direction == WebCore::ScrollRight)
        return !m_webView->client()->historyForwardListCount();

    ASSERT_NOT_REACHED();
    return true;
}
