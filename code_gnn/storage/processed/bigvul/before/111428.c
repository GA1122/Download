void WebPagePrivate::willDeferLoading()
{
    m_deferredTasksTimer.stop();
    m_client->willDeferLoading();
}
