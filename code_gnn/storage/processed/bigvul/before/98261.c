unsigned WebFrame::numberOfActiveAnimations() const
{
    if (!m_coreFrame)
        return 0;

    AnimationController* controller = m_coreFrame->animation();
    if (!controller)
        return 0;

    return controller->numberOfActiveAnimations();
}
