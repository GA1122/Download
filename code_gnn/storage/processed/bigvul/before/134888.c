    void loadURLInTopFrame(const WebURL& url)
    {
        FrameTestHelpers::loadFrame(m_helper.webView()->mainFrame(), url.string().utf8());
    }
