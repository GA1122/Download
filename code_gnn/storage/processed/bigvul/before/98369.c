String FrameLoaderClient::userAgent(const KURL&)
{
    WebKitWebSettings* settings = webkit_web_view_get_settings(getViewFromFrame(m_frame));
    return String::fromUTF8(webkit_web_settings_get_user_agent(settings));
}
