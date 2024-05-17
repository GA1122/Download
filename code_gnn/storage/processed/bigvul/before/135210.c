void Document::didUpdateSecurityOrigin()
{
    if (!m_frame)
        return;
    m_frame->updateSecurityOrigin(securityOrigin());
}
