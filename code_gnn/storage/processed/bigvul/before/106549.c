void WebPageProxy::goForward()
{
    if (isValid() && !canGoForward())
        return;

    WebBackForwardListItem* forwardItem = m_backForwardList->forwardItem();
    if (forwardItem)
        setPendingAPIRequestURL(forwardItem->url());

    if (!isValid()) {
        reattachToWebProcessWithItem(forwardItem);
        return;
    }

    SandboxExtension::Handle sandboxExtensionHandle;
    initializeSandboxExtensionHandle(KURL(KURL(), forwardItem->url()), sandboxExtensionHandle);
    process()->send(Messages::WebPage::GoForward(forwardItem->itemID(), sandboxExtensionHandle), m_pageID);
}
