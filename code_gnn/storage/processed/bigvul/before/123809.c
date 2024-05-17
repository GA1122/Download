EntrySync* EntrySync::getParent() const
{
    String parentPath = DOMFilePath::getDirectory(fullPath());
    return DirectoryEntrySync::create(m_fileSystem, parentPath);
}
