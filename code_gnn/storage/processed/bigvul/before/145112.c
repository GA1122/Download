    void testFrameFinalizedByTaskObserver2()
    {
        EXPECT_EQ(3, m_fakeImageBufferClient->frameCount());
        expectDisplayListEnabled(false);
        m_testSurface->getPicture();
        EXPECT_EQ(3, m_fakeImageBufferClient->frameCount());
        expectDisplayListEnabled(false);
        m_fakeImageBufferClient->fakeDraw();
        EXPECT_EQ(3, m_fakeImageBufferClient->frameCount());
        expectDisplayListEnabled(false);
    }
