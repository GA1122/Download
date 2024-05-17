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

            if (m_webPage->hasOpenedPopup())
                m_page->chrome()->client()->closePagePopup(0);
        }

        m_visible = visible;
        m_backingStore->d->updateSuspendScreenUpdateState();
    }

#if ENABLE(PAGE_VISIBILITY_API)
    setPageVisibilityState();
#endif
}
