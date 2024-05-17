int64_t WebPageProxy::spellDocumentTag()
{
    if (!m_hasSpellDocumentTag) {
        m_spellDocumentTag = TextChecker::uniqueSpellDocumentTag();
        m_hasSpellDocumentTag = true;
    }

    return m_spellDocumentTag;
}
