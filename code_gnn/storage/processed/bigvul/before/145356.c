SecurityContext::InsecureRequestsPolicy FrameLoader::getInsecureRequestsPolicy() const
{
    Frame* parentFrame = m_frame->tree().parent();
    if (!parentFrame)
        return SecurityContext::InsecureRequestsDoNotUpgrade;

    if (!parentFrame->isLocalFrame())
        return SecurityContext::InsecureRequestsDoNotUpgrade;

    ASSERT(toLocalFrame(parentFrame)->document());
    return toLocalFrame(parentFrame)->document()->getInsecureRequestsPolicy();
}
