void HTMLDocumentParser::pumpTokenizer(SynchronousMode mode)
{
    ASSERT(!isStopped());
    ASSERT(!isScheduledForResume());
    ASSERT(refCount() >= 2);

    PumpSession session(m_pumpSessionNestingLevel);

    InspectorInstrumentationCookie cookie = InspectorInstrumentation::willWriteHTML(document(), m_input.current().length(), m_input.current().currentLine().zeroBasedInt());

    while (canTakeNextToken(mode, session) && !session.needsYield) {
        if (!isParsingFragment())
            m_sourceTracker.start(m_input, m_tokenizer.get(), m_token);

        if (!m_tokenizer->nextToken(m_input.current(), m_token))
            break;

        if (!isParsingFragment()) {
            m_sourceTracker.end(m_input, m_tokenizer.get(), m_token);

            m_xssAuditor.filterToken(m_token);
        }

        constructTreeFromHTMLToken(m_token);
        ASSERT(m_token.isUninitialized());
    }

    ASSERT(refCount() >= 1);

    if (isStopped())
        return;

    if (session.needsYield)
        m_parserScheduler->scheduleForResume();

    if (isWaitingForScripts()) {
        ASSERT(m_tokenizer->state() == HTMLTokenizerState::DataState);
        if (!m_preloadScanner) {
            m_preloadScanner = adoptPtr(new HTMLPreloadScanner(document(), m_options));
            m_preloadScanner->appendToEnd(m_input.current());
        }
        m_preloadScanner->scan();
    }

    InspectorInstrumentation::didWriteHTML(cookie, m_input.current().currentLine().zeroBasedInt());
}
