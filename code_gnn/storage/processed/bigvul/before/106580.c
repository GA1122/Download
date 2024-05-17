void WebPageProxy::loadURLRequest(WebURLRequest* urlRequest)
{
    setPendingAPIRequestURL(urlRequest->resourceRequest().url());

    if (!isValid())
        reattachToWebProcess();

    SandboxExtension::Handle sandboxExtensionHandle;
    initializeSandboxExtensionHandle(urlRequest->resourceRequest().url(), sandboxExtensionHandle);
    process()->send(Messages::WebPage::LoadURLRequest(urlRequest->resourceRequest(), sandboxExtensionHandle), m_pageID);
}
