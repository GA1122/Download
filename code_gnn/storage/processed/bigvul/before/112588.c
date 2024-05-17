void Document::suspendScriptedAnimationControllerCallbacks()
{
#if ENABLE(REQUEST_ANIMATION_FRAME)
    if (m_scriptedAnimationController)
        m_scriptedAnimationController->suspend();
#endif
}
