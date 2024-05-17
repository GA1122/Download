void WebPagePrivate::resumeDocumentStyleRecalc()
{
    if (Document* document = m_mainFrame->document()) {
        if (m_documentChildNeedsStyleRecalc)
            document->setChildNeedsStyleRecalc();

        if (m_documentStyleRecalcPostponed)
            document->scheduleStyleRecalc();
    }

    m_documentChildNeedsStyleRecalc = false;
    m_documentStyleRecalcPostponed = false;
}
