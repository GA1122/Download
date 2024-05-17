PassOwnPtr<XSSInfo> XSSAuditor::filterToken(const FilterTokenRequest& request)
{
    ASSERT(m_state != Uninitialized);
    if (!m_isEnabled || m_xssProtection == AllowReflectedXSS)
        return nullptr;

    bool didBlockScript = false;
    if (request.token.type() == HTMLToken::StartTag)
        didBlockScript = filterStartToken(request);
    else if (m_scriptTagNestingLevel) {
        if (request.token.type() == HTMLToken::Character)
            didBlockScript = filterCharacterToken(request);
        else if (request.token.type() == HTMLToken::EndTag)
            filterEndToken(request);
    }

    if (didBlockScript) {
        bool didBlockEntirePage = (m_xssProtection == BlockReflectedXSS);
        OwnPtr<XSSInfo> xssInfo = XSSInfo::create(m_documentURL, didBlockEntirePage, m_didSendValidXSSProtectionHeader, m_didSendValidCSPHeader);
        return xssInfo.release();
    }
    return nullptr;
}
