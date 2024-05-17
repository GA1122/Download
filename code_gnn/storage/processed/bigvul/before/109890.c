void SecurityContext::setSecurityOrigin(PassRefPtr<SecurityOrigin> securityOrigin)
{
    m_securityOrigin = securityOrigin;
    m_haveInitializedSecurityOrigin = true;
}
