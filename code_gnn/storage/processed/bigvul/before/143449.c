void HTMLPreloadScanner::scanAndPreload(ResourcePreloader* preloader, const KURL& startingBaseElementURL, ViewportDescriptionWrapper* viewport)
{
    ASSERT(isMainThread());  

    TRACE_EVENT1("blink", "HTMLPreloadScanner::scan", "source_length", m_source.length());

    if (!startingBaseElementURL.isEmpty())
        m_scanner.setPredictedBaseElementURL(startingBaseElementURL);

    PreloadRequestStream requests;

    while (m_tokenizer->nextToken(m_source, m_token)) {
        if (m_token.type() == HTMLToken::StartTag)
            m_tokenizer->updateStateFor(attemptStaticStringCreation(m_token.name(), Likely8Bit));
        m_scanner.scan(m_token, m_source, requests, viewport);
        m_token.clear();
    }

    preloader->takeAndPreload(requests);
}
