WebPage::WebPage(WebPageClient* client, const BlackBerry::Platform::String& pageGroupName, const Platform::IntRect& rect)
{
    globalInitialize();
    d = new WebPagePrivate(this, client, rect);
    d->init(pageGroupName);
}
