bool HTMLDocumentParser::processingData() const
{
    return isScheduledForResume() || inPumpSession();
}
