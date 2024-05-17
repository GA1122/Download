void WebPagePrivate::toggleTextReflowIfEnabledForBlockZoomOnly(bool shouldEnableTextReflow)
{
    if (m_webPage->settings()->textReflowMode() == WebSettings::TextReflowEnabledOnlyForBlockZoom)
        m_page->settings()->setTextReflowEnabled(shouldEnableTextReflow);
}
