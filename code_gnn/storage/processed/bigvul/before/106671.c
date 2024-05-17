void WebPageProxy::viewWillEndLiveResize()
{
    if (!isValid())
        return;
    process()->send(Messages::WebPage::ViewWillEndLiveResize(), m_pageID);
}
