    void serialize(const char* url)
    {
        FrameTestHelpers::loadFrame(m_helper.webView()->mainFrame(), KURL(m_baseUrl, url).string().utf8().data());
        PageSerializer serializer(&m_resources, nullptr);

        serializer.setRewriteURLFolder(m_rewriteFolder);
        for (const auto& rewriteURL: m_rewriteURLs)
            serializer.registerRewriteURL(rewriteURL.key, rewriteURL.value);

        serializer.serialize(m_helper.webViewImpl()->mainFrameImpl()->frame()->page());
    }
