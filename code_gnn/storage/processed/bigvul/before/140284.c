bool GIFLZWContext::prepareToDecode()
{
    ASSERT(m_frameContext->isDataSizeDefined() && m_frameContext->isHeaderDefined());

    if (m_frameContext->dataSize() >= MAX_DICTIONARY_ENTRY_BITS)
        return false;
    clearCode = 1 << m_frameContext->dataSize();
    avail = clearCode + 2;
    oldcode = -1;
    codesize = m_frameContext->dataSize() + 1;
    codemask = (1 << codesize) - 1;
    datum = bits = 0;
    ipass = m_frameContext->interlaced() ? 1 : 0;
    irow = 0;

    const size_t maxBytes = MAX_DICTIONARY_ENTRIES - 1;

    rowBuffer.resize(m_frameContext->width() - 1 + maxBytes);
    rowIter = rowBuffer.begin();
    rowsRemaining = m_frameContext->height();

    for (int i = 0; i < clearCode; ++i) {
        suffix[i] = i;
        suffixLength[i] = 1;
    }
    return true;
}
