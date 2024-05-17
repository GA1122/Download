void FrameLoader::didInstallNewDocument(bool dispatchWindowObjectAvailable)
{
    ASSERT(m_frame);
    ASSERT(m_frame->document());

    m_frame->document()->setReadyState(Document::Loading);

    if (dispatchWindowObjectAvailable)
        dispatchDidClearDocumentOfWindowObject();

    m_frame->document()->initContentSecurityPolicy(m_documentLoader ? m_documentLoader->releaseContentSecurityPolicy() : ContentSecurityPolicy::create());

    if (m_provisionalItem && isBackForwardLoadType(m_loadType))
        m_frame->document()->setStateForNewFormElements(m_provisionalItem->documentState());
}
