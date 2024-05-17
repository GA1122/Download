void WebFrame::resumeAnimations()
{
    if (!m_coreFrame)
        return;

    AnimationController* controller = m_coreFrame->animation();
    if (!controller)
        return;

    controller->resumeAnimations();
}
