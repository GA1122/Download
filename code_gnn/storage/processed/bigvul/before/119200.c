SecurityOrigin* XMLHttpRequest::securityOrigin() const
{
    return m_securityOrigin ? m_securityOrigin.get() : scriptExecutionContext()->securityOrigin();
}
