void WebPageProxy::processDidBecomeUnresponsive()
{
    m_loaderClient.processDidBecomeUnresponsive(this);
}
