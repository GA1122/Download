void FrameLoader::commitProvisionalLoad()
{
    ASSERT(client()->hasWebView());

    if (m_frame->document()) {
        RefPtr<SecurityOrigin> securityOrigin = SecurityOrigin::create(m_provisionalDocumentLoader->request().url());
        m_provisionalDocumentLoader->timing().setHasSameOriginAsPreviousDocument(securityOrigin->canRequest(m_frame->document()->url()));
    }

    if (!prepareForCommit())
        return;

    if (isLoadingMainFrame()) {
        m_frame->page()->chromeClient().setEventListenerProperties(WebEventListenerClass::TouchStartOrMove, WebEventListenerProperties::Nothing);
        m_frame->page()->chromeClient().setEventListenerProperties(WebEventListenerClass::MouseWheel, WebEventListenerProperties::Nothing);
        m_frame->page()->chromeClient().setEventListenerProperties(WebEventListenerClass::TouchEndOrCancel, WebEventListenerProperties::Nothing);
    }

    client()->transitionToCommittedForNewPage();
    m_frame->navigationScheduler().cancel();
    m_frame->editor().clearLastEditCommand();

    if (!m_stateMachine.creatingInitialEmptyDocument()) {
        DOMWindow* window = m_frame->domWindow();
        window->setStatus(String());
        window->setDefaultStatus(String());
    }
}
