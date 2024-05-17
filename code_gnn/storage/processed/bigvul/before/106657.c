void WebPageProxy::terminateProcess()
{
    if (!isValid())
        return;

    process()->terminate();
}
