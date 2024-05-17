void Document::beginLifecycleUpdatesIfRenderingReady()
{
    if (!isActive())
        return;
    if (!isRenderingReady())
        return;
    if (LocalFrame* frame = this->frame()) {
        if (!frame->loader().stateMachine()->committedFirstRealDocumentLoad())
            return;
        if (frame->isMainFrame())
            frame->page()->chromeClient().beginLifecycleUpdates();
    }
}
