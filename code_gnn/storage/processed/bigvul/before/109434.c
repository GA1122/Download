PassRefPtr<ArrayBuffer> FileReaderLoader::arrayBufferResult() const
{
    ASSERT(m_readType == ReadAsArrayBuffer);

    if (!m_rawData || m_errorCode)
        return 0;

    if (isCompleted())
        return m_rawData;

    return ArrayBuffer::create(m_rawData.get());
}
