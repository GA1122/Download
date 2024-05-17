void WebPagePrivate::notifyAppActivationStateChange(ActivationStateType activationState)
{
    m_activationState = activationState;

#if ENABLE(PAGE_VISIBILITY_API)
    setPageVisibilityState();
#endif
}
