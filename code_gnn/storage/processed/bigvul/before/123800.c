DirectoryEntrySync* DOMFileSystemSync::root()
{
    return DirectoryEntrySync::create(this, DOMFilePath::root);
}
