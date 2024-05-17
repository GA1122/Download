void Document::cancelParsing()
{
    detachParser();
    setParsingState(FinishedParsing);
    setReadyState(Complete);
}
