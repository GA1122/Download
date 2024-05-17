    UseMockScrollbarSettings()
    {
        WebCore::Settings::setMockScrollbarsEnabled(true);
        WebCore::RuntimeEnabledFeatures::setOverlayScrollbarsEnabled(true);
        EXPECT_TRUE(WebCore::ScrollbarTheme::theme()->usesOverlayScrollbars());
    }
