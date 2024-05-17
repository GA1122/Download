void Document::explicitClose()
{
    if (RefPtrWillBeRawPtr<DocumentParser> parser = m_parser)
        parser->finish();

    if (!m_frame) {
        implicitClose();
        return;
    }

    m_frame->loader().checkCompleted();
}
