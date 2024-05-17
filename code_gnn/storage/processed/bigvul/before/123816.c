void provideLocalFileSystemToWorker(WorkerClients* clients, PassOwnPtr<FileSystemClient> client)
{
    clients->provideSupplement(LocalFileSystem::supplementName(), LocalFileSystem::create(client));
}
