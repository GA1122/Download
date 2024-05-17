void FileReaderLoader::didReceiveData(const char* data, int dataLength)
{
    ASSERT(data);
    ASSERT(dataLength > 0);

    if (m_errorCode)
        return;

    int length = dataLength;
    unsigned remainingBufferSpace = m_totalBytes - m_bytesLoaded;
    if (length > static_cast<long long>(remainingBufferSpace)) {
        if (m_totalBytes >= numeric_limits<unsigned>::max()) {
            failed(FileError::NOT_READABLE_ERR);
            return;
        }
        if (m_variableLength) {
            unsigned long long newLength = m_totalBytes * 2;
            if (newLength > numeric_limits<unsigned>::max())
                newLength = numeric_limits<unsigned>::max();
            RefPtr<ArrayBuffer> newData =
                ArrayBuffer::create(static_cast<unsigned>(newLength), 1);
            memcpy(static_cast<char*>(newData->data()), static_cast<char*>(m_rawData->data()), m_bytesLoaded);

            m_rawData = newData;
            m_totalBytes = static_cast<unsigned>(newLength);
        } else
            length = remainingBufferSpace;
    }

    if (length <= 0)
        return;

    memcpy(static_cast<char*>(m_rawData->data()) + m_bytesLoaded, data, length);
    m_bytesLoaded += length;

    m_isRawDataConverted = false;

    if (m_client)
        m_client->didReceiveData();
}
