    void testClearRect()
    {
        m_testSurface->initializeCurrentFrame();
        m_testSurface->getPicture();
        SkPaint clearPaint;
        clearPaint.setXfermodeMode(SkXfermode::kClear_Mode);
        m_imageBuffer->canvas()->drawRect(SkRect::MakeWH(m_testSurface->size().width(), m_testSurface->size().height()), clearPaint);
        m_fakeImageBufferClient->fakeDraw();
        EXPECT_EQ(1, m_fakeImageBufferClient->frameCount());
        m_testSurface->getPicture();
        EXPECT_EQ(2, m_fakeImageBufferClient->frameCount());
        expectDisplayListEnabled(true);
    }
