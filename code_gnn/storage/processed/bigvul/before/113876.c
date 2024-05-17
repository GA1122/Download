void HTMLDocumentParser::attemptToEnd()
{

    if (shouldDelayEnd()) {
        m_endWasDelayed = true;
        return;
    }
    prepareToStopParsing();
}
