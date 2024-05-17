void WebPage::platformPreferencesDidChange(const WebPreferencesStore& store)
{
    FontSmoothingLevel fontSmoothingLevel = static_cast<FontSmoothingLevel>(store.getUInt32ValueForKey(WebPreferencesKey::fontSmoothingLevelKey()));

#if PLATFORM(CG)
    FontSmoothingLevel adjustedLevel = fontSmoothingLevel;
    if (adjustedLevel == FontSmoothingLevelWindows)
        adjustedLevel = FontSmoothingLevelMedium;
    wkSetFontSmoothingLevel(adjustedLevel);
#endif

    m_page->settings()->setFontRenderingMode(fontSmoothingLevel == FontSmoothingLevelWindows ? AlternateRenderingMode : NormalRenderingMode);
}
