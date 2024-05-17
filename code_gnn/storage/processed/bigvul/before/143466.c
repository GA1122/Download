    void preloadRequestVerification(Resource::Type type, const char* url, const char* baseURL, int width, ReferrerPolicy referrerPolicy)
    {
        preloadRequestVerification(type, url, baseURL, width, ClientHintsPreferences());
        EXPECT_EQ(referrerPolicy, m_preloadRequest->getReferrerPolicy());
    }
