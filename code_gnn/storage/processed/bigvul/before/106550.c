void WebPageProxy::goToBackForwardItem(WebBackForwardListItem* item)
{
    if (!isValid()) {
        reattachToWebProcessWithItem(item);
        return;
    }
    
    setPendingAPIRequestURL(item->url());

    SandboxExtension::Handle sandboxExtensionHandle;
    initializeSandboxExtensionHandle(KURL(KURL(), item->url()), sandboxExtensionHandle);
    process()->send(Messages::WebPage::GoToBackForwardItem(item->itemID(), sandboxExtensionHandle), m_pageID);
}
