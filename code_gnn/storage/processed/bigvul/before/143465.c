    void preloadRequestVerification(Resource::Type type, const char* url, const char* baseURL, int width, const ClientHintsPreferences& preferences)
    {
        if (!url) {
            EXPECT_FALSE(m_preloadRequest);
            return;
        }
        ASSERT(m_preloadRequest.get());
        EXPECT_FALSE(m_preloadRequest->isPreconnect());
        EXPECT_EQ(type, m_preloadRequest->resourceType());
        EXPECT_STREQ(url, m_preloadRequest->resourceURL().ascii().data());
        EXPECT_STREQ(baseURL, m_preloadRequest->baseURL().getString().ascii().data());
        EXPECT_EQ(width, m_preloadRequest->resourceWidth());
        EXPECT_EQ(preferences.shouldSendDPR(), m_preloadRequest->preferences().shouldSendDPR());
        EXPECT_EQ(preferences.shouldSendResourceWidth(), m_preloadRequest->preferences().shouldSendResourceWidth());
        EXPECT_EQ(preferences.shouldSendViewportWidth(), m_preloadRequest->preferences().shouldSendViewportWidth());
    }
