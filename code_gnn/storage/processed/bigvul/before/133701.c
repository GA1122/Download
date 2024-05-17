bool XSSAuditor::filterCharacterToken(const FilterTokenRequest& request)
{
    ASSERT(m_scriptTagNestingLevel);
    ASSERT(m_state != Uninitialized);
    if (m_state == PermittingAdjacentCharacterTokens)
        return false;

    if ((m_state == SuppressingAdjacentCharacterTokens)
        || (m_scriptTagFoundInRequest && isContainedInRequest(decodedSnippetForJavaScript(request)))) {
        request.token.eraseCharacters();
        request.token.appendToCharacter(' ');  
        m_state = SuppressingAdjacentCharacterTokens;
        return true;
    }

    m_state = PermittingAdjacentCharacterTokens;
    return false;
}
