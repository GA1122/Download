SecurityOrigin* DocumentThreadableLoader::getSecurityOrigin() const
{
    return m_securityOrigin ? m_securityOrigin.get() : document().getSecurityOrigin();
}
