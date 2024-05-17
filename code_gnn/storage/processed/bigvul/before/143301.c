void Document::willInsertBody()
{
    if (frame())
        frame()->loader().client()->dispatchWillInsertBody();
    beginLifecycleUpdatesIfRenderingReady();
}
