void WebPageProxy::findString(const String& string, FindOptions options, unsigned maxMatchCount)
{
    process()->send(Messages::WebPage::FindString(string, options, maxMatchCount), m_pageID);
}
