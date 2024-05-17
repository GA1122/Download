WebInsecureRequestPolicy FrameLoader::getInsecureRequestPolicy() const
{
    Frame* parentFrame = m_frame->tree().parent();
    if (!parentFrame)
        return kLeaveInsecureRequestsAlone;

    return parentFrame->securityContext()->getInsecureRequestPolicy();
}
