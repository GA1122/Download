void HTMLDocumentParser::resumeParsingAfterYield()
{
    RefPtr<HTMLDocumentParser> protect(this);

    pumpTokenizer(AllowYield);
    endIfDelayed();
}
