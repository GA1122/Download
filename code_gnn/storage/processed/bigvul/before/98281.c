void FrameLoaderClient::committedLoad(WebCore::DocumentLoader* loader, const char* data, int length)
{
    if (!m_pluginView) {
        ASSERT(loader->frame());
        String encoding = loader->overrideEncoding();
        bool userChosen = !encoding.isNull();
        if (!userChosen)
            encoding = loader->response().textEncodingName();

        FrameLoader* frameLoader = loader->frameLoader();
        frameLoader->setEncoding(encoding, userChosen);
        if (data)
            frameLoader->addData(data, length);
    }

    if (m_pluginView) {
        if (!m_hasSentResponseToPlugin) {
            m_pluginView->didReceiveResponse(loader->response());
            m_hasSentResponseToPlugin = true;
        }

        if (!m_pluginView)
            return;

        m_pluginView->didReceiveData(data, length);
    }
}
