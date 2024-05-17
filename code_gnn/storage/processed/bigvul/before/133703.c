void XSSAuditor::filterEndToken(const FilterTokenRequest& request)
{
    ASSERT(m_scriptTagNestingLevel);
    m_state = FilteringTokens;
    if (hasName(request.token, scriptTag)) {
        m_scriptTagNestingLevel--;
        ASSERT(request.shouldAllowCDATA || !m_scriptTagNestingLevel);
    }
}
