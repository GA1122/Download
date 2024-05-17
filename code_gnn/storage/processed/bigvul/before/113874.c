void HTMLDocumentParser::append(const SegmentedString& source)
{
    if (isStopped())
        return;

    RefPtr<HTMLDocumentParser> protect(this);

    if (m_preloadScanner) {
        if (m_input.current().isEmpty() && !isWaitingForScripts()) {
            m_preloadScanner.clear();
        } else {
            m_preloadScanner->appendToEnd(source);
            if (isWaitingForScripts())
                m_preloadScanner->scan();
        }
    }

    m_input.appendToEnd(source);

    if (inPumpSession()) {
        return;
    }

    pumpTokenizerIfPossible(AllowYield);

    endIfDelayed();
}
