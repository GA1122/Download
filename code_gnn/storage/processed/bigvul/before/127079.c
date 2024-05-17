bool ChromeClientImpl::shouldReportDetailedMessageForSource(const String& url)
{
    return m_webView->client() && m_webView->client()->shouldReportDetailedMessageForSource(url);
}
