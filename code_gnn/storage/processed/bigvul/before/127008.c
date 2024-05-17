void ChromeClientImpl::closeWindowSoon()
{
    m_webView->page()->clearPageGroup();

    m_webView->mainFrame()->stopLoading();

    if (m_webView->client())
        m_webView->client()->closeWidgetSoon();
}
