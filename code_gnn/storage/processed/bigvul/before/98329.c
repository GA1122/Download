void FrameLoaderClient::finishedLoading(WebCore::DocumentLoader* documentLoader)
{
    if (!m_pluginView)
        committedLoad(documentLoader, 0, 0);
    else {
        m_pluginView->didFinishLoading();
        m_pluginView = 0;
        m_hasSentResponseToPlugin = false;
    }
}
