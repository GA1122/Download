    void preconnectRequestVerification(const String& host, CrossOriginAttributeValue crossOrigin)
    {
        if (!host.isNull()) {
            EXPECT_TRUE(m_preloadRequest->isPreconnect());
            EXPECT_STREQ(m_preloadRequest->resourceURL().ascii().data(), host.ascii().data());
            EXPECT_EQ(m_preloadRequest->crossOrigin(), crossOrigin);
        }
    }
