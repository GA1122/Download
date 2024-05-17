String HTMLDocumentParser::sourceForToken(const HTMLToken& token)
{
    return m_sourceTracker.sourceForToken(token);
}
