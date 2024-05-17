    AutoInstallCurrentThreadPlatformMock()
    {
        m_oldPlatform = Platform::current();
        Platform::initialize(&m_mockPlatform);
    }
