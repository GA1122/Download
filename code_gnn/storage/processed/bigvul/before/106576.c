void WebPageProxy::loadAlternateHTMLString(const String& htmlString, const String& baseURL, const String& unreachableURL)
{
    if (!isValid())
        reattachToWebProcess();

    if (m_mainFrame)
        m_mainFrame->setUnreachableURL(unreachableURL);

    process()->send(Messages::WebPage::LoadAlternateHTMLString(htmlString, baseURL, unreachableURL), m_pageID);
}
