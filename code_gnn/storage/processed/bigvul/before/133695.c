String XSSAuditor::decodedSnippetForName(const FilterTokenRequest& request)
{
    return fullyDecodeString(request.sourceTracker.sourceForToken(request.token), m_encoding).substring(0, request.token.name().size() + 1);
}
