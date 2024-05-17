    void expectDisplayListEnabled(bool displayListEnabled)
    {
        EXPECT_EQ(displayListEnabled, (bool)m_testSurface->m_currentFrame.get());
        EXPECT_EQ(!displayListEnabled, (bool)m_testSurface->m_fallbackSurface.get());
        int expectedSurfaceCreationCount = displayListEnabled ? 0 : 1;
        EXPECT_EQ(expectedSurfaceCreationCount, m_surfaceFactory->createSurfaceCount());
    }
