 void XMLHttpRequest::dropProtectionSoon()
{
    if (m_protectionTimer.isActive())
        return;
    m_protectionTimer.startOneShot(0);
}
