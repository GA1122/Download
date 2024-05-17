EntrySync::EntrySync(DOMFileSystemBase* fileSystem, const String& fullPath)
    : EntryBase(fileSystem, fullPath)
{
    ScriptWrappable::init(this);
}
