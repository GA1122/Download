    void testAnimatedWithClear()
    {
        m_testSurface->initializeCurrentFrame();
        m_testSurface->getPicture();
        m_testSurface->willOverwriteCanvas();
        m_fakeImageBufferClient->fakeDraw();
        EXPECT_EQ(1, m_fakeImageBufferClient->frameCount());
        m_testSurface->getPicture();
        EXPECT_EQ(2, m_fakeImageBufferClient->frameCount());
        expectDisplayListEnabled(true);
        m_fakeImageBufferClient->fakeDraw();
        m_testSurface->willOverwriteCanvas();
        EXPECT_EQ(2, m_fakeImageBufferClient->frameCount());
        m_testSurface->getPicture();
        EXPECT_EQ(3, m_fakeImageBufferClient->frameCount());
        expectDisplayListEnabled(true);
    }
