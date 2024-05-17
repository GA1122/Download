void HTMLPreloadScanner::scan()
{
    m_predictedBaseElementURL = m_document->baseElementURL();

    while (m_tokenizer->nextToken(m_source, m_token)) {
        processToken();
        m_token.clear();
    }
}
