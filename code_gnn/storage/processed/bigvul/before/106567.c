void WebPageProxy::initializeSandboxExtensionHandle(const KURL& url, SandboxExtension::Handle& sandboxExtensionHandle)
{
    if (!url.isLocalFile())
        return;

    if (WebInspectorProxy::isInspectorPage(this))
        return;

    SandboxExtension::createHandle("/", SandboxExtension::ReadOnly, sandboxExtensionHandle);
}
