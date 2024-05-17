    void testNoFallbackWithClear()
    {
        m_testSurface->initializeCurrentFrame();
        m_testSurface->willOverwriteCanvas();
        m_testSurface->getPicture();
        EXPECT_EQ(1, m_fakeImageBufferClient->frameCount());
        expectDisplayListEnabled(true);
    }
