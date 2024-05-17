bool FrameLoader::shouldEnforceStrictMixedContentChecking() const
{
    Frame* parentFrame = m_frame->tree().parent();
    if (!parentFrame)
        return false;

    return parentFrame->securityContext()->shouldEnforceStrictMixedContentChecking();
}
