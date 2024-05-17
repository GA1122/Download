void FileReaderLoader::didReceiveResponse(unsigned long, const ResourceResponse& response)
{
    if (response.httpStatusCode() != 200) {
        failed(httpStatusCodeToErrorCode(response.httpStatusCode()));
        return;
    }

    unsigned long long length = response.expectedContentLength();

    if (length > INT_MAX) {
        m_variableLength = true;
        if (m_hasRange)
            length = 1 + m_rangeEnd - m_rangeStart;
        else
            length = defaultBufferLength;
    }

    if (length > numeric_limits<unsigned>::max()) {
        failed(FileError::NOT_READABLE_ERR);
        return;
    }

    ASSERT(!m_rawData);
    m_rawData = ArrayBuffer::create(static_cast<unsigned>(length), 1);

    if (!m_rawData) {
        failed(FileError::NOT_READABLE_ERR);
        return;
    }

    m_totalBytes = static_cast<unsigned>(length);

    if (m_client)
        m_client->didStartLoading();
}
