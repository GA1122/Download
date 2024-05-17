        ~ProxyPlatform()
        {
            blink::Platform::initialize(m_platform);
        }
