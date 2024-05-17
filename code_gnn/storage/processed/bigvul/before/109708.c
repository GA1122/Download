void Document::initContentSecurityPolicy(const ContentSecurityPolicyResponseHeaders& headers)
{
    if (m_frame && m_frame->tree()->parent() && (shouldInheritSecurityOriginFromOwner(m_url) || isPluginDocument()))
        contentSecurityPolicy()->copyStateFrom(m_frame->tree()->parent()->document()->contentSecurityPolicy());
    contentSecurityPolicy()->didReceiveHeaders(headers);
}
