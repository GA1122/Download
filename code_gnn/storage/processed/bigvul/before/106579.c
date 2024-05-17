void WebPageProxy::loadURL(const String& url)
{
    setPendingAPIRequestURL(url);

    if (!isValid())
        reattachToWebProcess();

    SandboxExtension::Handle sandboxExtensionHandle;
    initializeSandboxExtensionHandle(KURL(KURL(), url), sandboxExtensionHandle);
    process()->send(Messages::WebPage::LoadURL(url, sandboxExtensionHandle), m_pageID);
}
