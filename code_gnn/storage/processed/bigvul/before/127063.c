void ChromeClientImpl::scheduleCompositingLayerFlush()
{
    m_webView->scheduleCompositingLayerSync();
}
