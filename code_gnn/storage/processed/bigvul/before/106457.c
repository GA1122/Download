void WebPageProxy::countStringMatches(const String& string, FindOptions options, unsigned maxMatchCount)
{
    process()->send(Messages::WebPage::CountStringMatches(string, options, maxMatchCount), m_pageID);
}
