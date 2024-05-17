EntrySync* EntrySync::create(EntryBase* entry)
{
    if (entry->isFile())
        return FileEntrySync::create(entry->m_fileSystem, entry->m_fullPath);
    return DirectoryEntrySync::create(entry->m_fileSystem, entry->m_fullPath);
}
