void Document::initContentSecurityPolicy(PassRefPtrWillBeRawPtr<ContentSecurityPolicy> csp)
{
    setContentSecurityPolicy(csp ? csp : ContentSecurityPolicy::create());
    if (m_frame && m_frame->tree().parent() && m_frame->tree().parent()->isLocalFrame()) {
        ContentSecurityPolicy* parentCSP = toLocalFrame(m_frame->tree().parent())->document()->contentSecurityPolicy();
        if (shouldInheritSecurityOriginFromOwner(m_url)) {
            contentSecurityPolicy()->copyStateFrom(parentCSP);
        } else if (isPluginDocument()) {
            contentSecurityPolicy()->copyPluginTypesFrom(parentCSP);
        }
    }
    contentSecurityPolicy()->bindToExecutionContext(this);
}
