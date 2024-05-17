void Document::initContentSecurityPolicy()
{
    if (!m_frame->tree()->parent() || (!shouldInheritSecurityOriginFromOwner(m_url) && !isPluginDocument()))
        return;

    contentSecurityPolicy()->copyStateFrom(m_frame->tree()->parent()->document()->contentSecurityPolicy());
}
