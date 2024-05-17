void WebSettingsImpl::setDefaultFontSize(int size)
{
    m_settings->setDefaultFontSize(size);
#if defined(OS_WIN)
    WebCore::RenderThemeChromiumWin::setDefaultFontSize(size);
#endif
}
