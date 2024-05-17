PassOwnPtrWillBeRawPtr<LocalFileSystem> LocalFileSystem::create(PassOwnPtr<FileSystemClient> client)
{
    return adoptPtrWillBeNoop(new LocalFileSystem(client));
}
