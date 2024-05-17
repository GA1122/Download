void FrameLoader::init()
{
    updateSandboxFlags();

    m_stateMachine.advanceTo(FrameLoaderStateMachine::CreatingInitialEmptyDocument);
    setPolicyDocumentLoader(m_client->createDocumentLoader(ResourceRequest(KURL(ParsedURLString, "")), SubstituteData()).get());
    setProvisionalDocumentLoader(m_policyDocumentLoader.get());
    setState(FrameStateProvisional);
    m_provisionalDocumentLoader->setResponse(ResourceResponse(KURL(), "text/html", 0, String(), String()));
    m_provisionalDocumentLoader->finishedLoading();
    writer()->begin(KURL(), false);
    writer()->end();
    m_frame->document()->cancelParsing();
    m_stateMachine.advanceTo(FrameLoaderStateMachine::DisplayingInitialEmptyDocument);
    m_didCallImplicitClose = true;
}
