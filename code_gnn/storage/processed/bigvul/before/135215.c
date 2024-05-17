void Document::dispatchEventsForPrinting()
{
    if (!m_scriptedAnimationController)
        return;
    m_scriptedAnimationController->dispatchEventsAndCallbacksForPrinting();
}
