void provideLocalFileSystemTo(LocalFrame& frame, PassOwnPtr<FileSystemClient> client)
{
    frame.provideSupplement(LocalFileSystem::supplementName(), LocalFileSystem::create(client));
}
