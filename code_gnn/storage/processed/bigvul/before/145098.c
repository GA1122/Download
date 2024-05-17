    RecordingImageBufferSurfaceTest()
    {
        OwnPtr<MockSurfaceFactory> surfaceFactory = adoptPtr(new MockSurfaceFactory());
        m_surfaceFactory = surfaceFactory.get();
        OwnPtr<RecordingImageBufferSurface> testSurface = adoptPtr(new RecordingImageBufferSurface(IntSize(10, 10), surfaceFactory.release()));
        m_testSurface = testSurface.get();
        m_imageBuffer = ImageBuffer::create(testSurface.release());
        EXPECT_FALSE(!m_imageBuffer);
        m_fakeImageBufferClient = adoptPtr(new FakeImageBufferClient(m_imageBuffer.get()));
        m_imageBuffer->setClient(m_fakeImageBufferClient.get());
    }
