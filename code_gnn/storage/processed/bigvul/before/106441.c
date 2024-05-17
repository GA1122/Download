bool WebPageProxy::canGoBack() const
{
    return m_backForwardList->backItem();
}
