bool WebFrame::pauseAnimationOnElementWithId(const String& animationName, const String& elementID, double time)
{
    if (!m_coreFrame)
        return false;

    AnimationController* controller = m_coreFrame->animation();
    if (!controller)
        return false;

    if (!m_coreFrame->document())
        return false;

    Node* coreNode = m_coreFrame->document()->getElementById(elementID);
    if (!coreNode || !coreNode->renderer())
        return false;

    return controller->pauseAnimationAtTime(coreNode->renderer(), animationName, time);
}
