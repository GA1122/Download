    ~UseMockScrollbarSettings()
    {
        WebCore::Settings::setMockScrollbarsEnabled(false);
        WebCore::RuntimeEnabledFeatures::setOverlayScrollbarsEnabled(false);
    }
