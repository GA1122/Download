PassRefPtr<WebPageProxy> WebPageProxy::create(PageClient* pageClient, WebContext* context, WebPageGroup* pageGroup, uint64_t pageID)
{
    return adoptRef(new WebPageProxy(pageClient, context, pageGroup, pageID));
}
