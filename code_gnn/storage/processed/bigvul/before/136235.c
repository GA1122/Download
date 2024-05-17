     WebPageSerializerTest() : m_supportedSchemes(static_cast<size_t>(3))
    {
        m_supportedSchemes[0] = "http";
        m_supportedSchemes[1] = "https";
        m_supportedSchemes[2] = "file";
    }
