    ReadableStreamReaderTest()
        : m_page(DummyPageHolder::create(IntSize(1, 1)))
        , m_stream(new StringStream(new NoopUnderlyingSource, new PermissiveStrategy))
    {
        m_stream->didSourceStart();
    }
