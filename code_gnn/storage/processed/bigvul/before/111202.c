void WebPage::load(const BlackBerry::Platform::String& url, const BlackBerry::Platform::String& networkToken, bool isInitial)
{
    d->load(url, networkToken, "GET", Platform::NetworkRequest::UseProtocolCachePolicy, 0, 0, 0, 0, isInitial, false);
}
