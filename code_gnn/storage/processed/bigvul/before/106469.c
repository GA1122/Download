void WebPageProxy::didChangeProgress(double value)
{
    m_estimatedProgress = value;

    m_loaderClient.didChangeProgress(this);
}
