bool FileReaderLoader::isCompleted() const
{
    return m_bytesLoaded == m_totalBytes;
}
