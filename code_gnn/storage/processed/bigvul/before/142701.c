void FrameLoader::replaceDocumentWhileExecutingJavaScriptURL(const String& source, Document* ownerDocument)
{
    if (!m_frame->document()->loader() || m_frame->document()->pageDismissalEventBeingDispatched() != Document::NoDismissal)
        return;

    DocumentLoader* documentLoader(m_frame->document()->loader());

    UseCounter::count(*m_frame->document(), UseCounter::ReplaceDocumentViaJavaScriptURL);

    DocumentInit init(ownerDocument, m_frame->document()->url(), m_frame);
    init.withNewRegistrationContext();

    stopAllLoaders();
    SubframeLoadingDisabler disabler(m_frame->document());
    m_frame->detachChildren();
    m_frame->document()->detach();
    clear();

    if (!m_frame->page())
        return;

    client()->transitionToCommittedForNewPage();
    documentLoader->replaceDocumentWhileExecutingJavaScriptURL(init, source);
}
