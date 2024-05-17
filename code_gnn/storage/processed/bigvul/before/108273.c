void FrameLoader::prepareForCachedPageRestore()
{
    ASSERT(!m_frame->tree()->parent());
    ASSERT(m_frame->page());
    ASSERT(m_frame->page()->mainFrame() == m_frame);

    m_frame->redirectScheduler()->cancel();

    closeURL();
    
    if (m_frame->script()->canExecuteScripts(NotAboutToExecuteScript)) {
        if (DOMWindow* window = m_frame->existingDOMWindow()) {
            window->setStatus(String());
            window->setDefaultStatus(String());
        }
    }
}
