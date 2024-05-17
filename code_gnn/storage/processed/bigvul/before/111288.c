void WebPagePrivate::postponeDocumentStyleRecalc()
{
    if (Document* document = m_mainFrame->document()) {
        m_documentChildNeedsStyleRecalc = document->childNeedsStyleRecalc();
        document->clearChildNeedsStyleRecalc();

        m_documentStyleRecalcPostponed = document->isPendingStyleRecalc();
        document->unscheduleStyleRecalc();
    }
}
