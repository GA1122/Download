SecurityContext::InsecureNavigationsSet* FrameLoader::insecureNavigationsToUpgrade() const
{
    ASSERT(m_frame);
    Frame* parentFrame = m_frame->tree().parent();
    if (!parentFrame)
        return nullptr;

    if (!parentFrame->isLocalFrame())
        return nullptr;

    ASSERT(toLocalFrame(parentFrame)->document());
    return toLocalFrame(parentFrame)->document()->insecureNavigationsToUpgrade();
}
