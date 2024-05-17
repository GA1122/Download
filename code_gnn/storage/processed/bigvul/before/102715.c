bool CCThreadProxy::initializeLayerRenderer()
{
    TRACE_EVENT("CCThreadProxy::initializeLayerRenderer", this, 0);
    RefPtr<GraphicsContext3D> context = m_layerTreeHost->createLayerTreeHostContext3D();
    if (!context)
        return false;
    ASSERT(context->hasOneRef());

    GraphicsContext3D* contextPtr = context.release().leakRef();
    ASSERT(contextPtr->hasOneRef());

    CCCompletionEvent completion;
    bool initializeSucceeded = false;
    LayerRendererCapabilities capabilities;
    s_ccThread->postTask(createCCThreadTask(this, &CCThreadProxy::initializeLayerRendererOnCCThread,
                                          AllowCrossThreadAccess(contextPtr), AllowCrossThreadAccess(&completion),
                                          AllowCrossThreadAccess(&initializeSucceeded), AllowCrossThreadAccess(&capabilities),
                                          AllowCrossThreadAccess(&m_compositorIdentifier)));
    completion.wait();

    if (initializeSucceeded)
        m_layerRendererCapabilitiesMainThreadCopy = capabilities;
    return initializeSucceeded;
}
