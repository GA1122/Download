void WebPagePrivate::setPageVisibilityState()
{
    if (m_page->defersLoading())
        m_deferredTasks.append(adoptPtr(new DeferredTaskSetPageVisibilityState(this)));
    else {
        DeferredTaskSetPageVisibilityState::finishOrCancel(this);

        static bool s_initialVisibilityState = true;

        m_page->setVisibilityState(m_visible && m_activationState == ActivationActive ? PageVisibilityStateVisible : PageVisibilityStateHidden, s_initialVisibilityState);
        s_initialVisibilityState = false;
    }
}
