    ~WebFrameCSSCallbackTest()
    {
        EXPECT_EQ(1U, m_client.m_matchedSelectors.size());
    }
