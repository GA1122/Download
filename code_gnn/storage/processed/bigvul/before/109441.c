void FileReaderLoader::didFinishLoading(unsigned long, double)
{
    if (m_variableLength && m_totalBytes > m_bytesLoaded) {
        RefPtr<ArrayBuffer> newData = m_rawData->slice(0, m_bytesLoaded);

        m_rawData = newData;
        m_totalBytes = m_bytesLoaded;
    }
    cleanup();
    if (m_client)
        m_client->didFinishLoading();
}
