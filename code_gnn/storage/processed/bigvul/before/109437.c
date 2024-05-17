void FileReaderLoader::cleanup()
{
    m_loader = 0;

    if (m_errorCode) {
        m_rawData = 0;
        m_stringResult = "";
    }
}
