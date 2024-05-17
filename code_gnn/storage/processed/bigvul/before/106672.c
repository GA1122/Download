void WebPageProxy::viewWillStartLiveResize()
{
    if (!isValid())
        return;
    process()->send(Messages::WebPage::ViewWillStartLiveResize(), m_pageID);
}
