    virtual void TearDown()
    {
        Platform::current()->unitTestSupport()->unregisterAllMockedURLs();
        m_webViewImpl->close();
        m_webViewImpl = 0;
        m_mainFrame->close();
        m_mainFrame = 0;
    }
