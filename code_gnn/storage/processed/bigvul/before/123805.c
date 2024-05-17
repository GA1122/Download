DirectoryReaderSync* DirectoryEntrySync::createReader()
{
    return DirectoryReaderSync::create(m_fileSystem, m_fullPath);
}
