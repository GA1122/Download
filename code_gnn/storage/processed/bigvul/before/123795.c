DOMFileSystemSync* DOMFileSystemSync::create(DOMFileSystemBase* fileSystem)
{
    return new DOMFileSystemSync(fileSystem->m_context, fileSystem->name(), fileSystem->type(), fileSystem->rootURL());
}
