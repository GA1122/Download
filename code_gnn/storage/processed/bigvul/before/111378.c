void WebPagePrivate::setVisible(bool visible)
{
    if (visible != m_visible) {
        if (visible) {
            if (m_mainFrame)
                m_mainFrame->animation()->resumeAnimations();
            if (m_page->scriptedAnimationsSuspended())
                m_page->resumeScriptedAnimations();
        } else {
            if (m_mainFrame)
                m_mainFrame->animation()->suspendAnimations();
            if (!m_page->scriptedAnimationsSuspended())
                m_page->suspendScriptedAnimations();
        }

        m_visible = visible;
    }

#if ENABLE(PAGE_VISIBILITY_API)
    setPageVisibilityState();
#endif
}
