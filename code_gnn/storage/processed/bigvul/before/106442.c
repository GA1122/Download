bool WebPageProxy::canGoForward() const
{
    return m_backForwardList->forwardItem();
}
