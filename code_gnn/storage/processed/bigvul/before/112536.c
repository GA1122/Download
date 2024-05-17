int Document::requestAnimationFrame(PassRefPtr<RequestAnimationFrameCallback> callback)
{
    if (!m_scriptedAnimationController) {
#if USE(REQUEST_ANIMATION_FRAME_DISPLAY_MONITOR)
        m_scriptedAnimationController = ScriptedAnimationController::create(this, page() ? page()->displayID() : 0);
#else
        m_scriptedAnimationController = ScriptedAnimationController::create(this, 0);
#endif
        if (!page() || page()->scriptedAnimationsSuspended())
            m_scriptedAnimationController->suspend();
    }

    return m_scriptedAnimationController->registerCallback(callback);
}
