void FileReaderLoader::failed(int errorCode)
{
    m_errorCode = errorCode;
    cleanup();
    if (m_client)
        m_client->didFail(m_errorCode);
}
