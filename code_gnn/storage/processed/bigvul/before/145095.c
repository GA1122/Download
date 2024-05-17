    FakeImageBufferClient(ImageBuffer* imageBuffer)
        : m_isDirty(false)
        , m_imageBuffer(imageBuffer)
        , m_frameCount(0)
    { }
