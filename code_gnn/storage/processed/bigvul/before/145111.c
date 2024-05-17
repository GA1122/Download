    void testFrameFinalizedByTaskObserver1()
    {
        m_testSurface->initializeCurrentFrame();
        expectDisplayListEnabled(true);
        m_testSurface->getPicture();
        EXPECT_EQ(1, m_fakeImageBufferClient->frameCount());
        expectDisplayListEnabled(true);
        m_fakeImageBufferClient->fakeDraw();
        EXPECT_EQ(1, m_fakeImageBufferClient->frameCount());
        expectDisplayListEnabled(true);
        m_testSurface->getPicture();
        EXPECT_EQ(2, m_fakeImageBufferClient->frameCount());
        expectDisplayListEnabled(true);
        m_fakeImageBufferClient->fakeDraw();
        EXPECT_EQ(2, m_fakeImageBufferClient->frameCount());
        expectDisplayListEnabled(true);
    }
