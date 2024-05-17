void WebPageProxy::reattachToWebProcessWithItem(WebBackForwardListItem* item)
{
    if (item && item != m_backForwardList->currentItem())
        m_backForwardList->goToItem(item);
    
    reattachToWebProcess();

    if (!item)
        return;

    SandboxExtension::Handle sandboxExtensionHandle;
    initializeSandboxExtensionHandle(KURL(KURL(), item->url()), sandboxExtensionHandle);
    process()->send(Messages::WebPage::GoToBackForwardItem(item->itemID(), sandboxExtensionHandle), m_pageID);
}
