void HTMLDocumentParser::resumeParsingAfterScriptExecution()
{
    ASSERT(!isExecutingScript());
    ASSERT(!isWaitingForScripts());

    m_insertionPreloadScanner.clear();
    pumpTokenizerIfPossible(AllowYield);
    endIfDelayed();
}
