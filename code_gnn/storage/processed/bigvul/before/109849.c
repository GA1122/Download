void Document::suspendScriptedAnimationControllerCallbacks()
{
    if (m_scriptedAnimationController)
        m_scriptedAnimationController->suspend();
}
