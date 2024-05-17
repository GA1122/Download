SandboxFlags FrameLoader::effectiveSandboxFlags() const
{
    SandboxFlags flags = m_forcedSandboxFlags;
    if (FrameOwner* frameOwner = m_frame->owner())
        flags |= frameOwner->getSandboxFlags();
    if (Frame* parentFrame = m_frame->tree().parent())
        flags |= parentFrame->securityContext()->getSandboxFlags();
    return flags;
}
