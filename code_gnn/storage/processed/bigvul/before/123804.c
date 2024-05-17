DirectoryEntrySync::DirectoryEntrySync(DOMFileSystemBase* fileSystem, const String& fullPath)
    : EntrySync(fileSystem, fullPath)
{
    ScriptWrappable::init(this);
}
