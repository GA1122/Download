void Document::processHttpEquivAcceptCH(const AtomicString& content)
{
    if (frame())
        handleAcceptClientHintsHeader(content, m_clientHintsPreferences);
}
