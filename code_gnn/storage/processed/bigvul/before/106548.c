void WebPageProxy::goBack()
{
    if (isValid() && !canGoBack())
        return;

    WebBackForwardListItem* backItem = m_backForwardList->backItem();
    if (backItem)
        setPendingAPIRequestURL(backItem->url());

    if (!isValid()) {
        reattachToWebProcessWithItem(backItem);
        return;
    }

    SandboxExtension::Handle sandboxExtensionHandle;
    initializeSandboxExtensionHandle(KURL(KURL(), backItem->url()), sandboxExtensionHandle);
    process()->send(Messages::WebPage::GoBack(backItem->itemID(), sandboxExtensionHandle), m_pageID);
}
