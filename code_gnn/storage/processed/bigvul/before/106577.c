void WebPageProxy::loadHTMLString(const String& htmlString, const String& baseURL)
{
    if (!isValid())
        reattachToWebProcess();

    process()->send(Messages::WebPage::LoadHTMLString(htmlString, baseURL), m_pageID);
}
