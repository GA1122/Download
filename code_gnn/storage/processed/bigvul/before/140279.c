bool GIFFrameContext::decode(const unsigned char* data, size_t length, blink::GIFImageDecoder* client, bool* frameDecoded)
{
    m_localColorMap.buildTable(data, length);

    *frameDecoded = false;
    if (!m_lzwContext) {
        if (!isDataSizeDefined() || !isHeaderDefined())
            return true;

        m_lzwContext = adoptPtr(new GIFLZWContext(client, this));
        if (!m_lzwContext->prepareToDecode()) {
            m_lzwContext.clear();
            return false;
        }

        m_currentLzwBlock = 0;
    }

    while (m_currentLzwBlock < m_lzwBlocks.size() && m_lzwContext->hasRemainingRows()) {
        size_t blockPosition = m_lzwBlocks[m_currentLzwBlock].blockPosition;
        size_t blockSize = m_lzwBlocks[m_currentLzwBlock].blockSize;
        if (blockPosition + blockSize > length)
            return false;
        if (!m_lzwContext->doLZW(data + blockPosition, blockSize))
            return false;
        ++m_currentLzwBlock;
    }

    if (isComplete()) {
        *frameDecoded = true;
        m_lzwContext.clear();
    }
    return true;
}
