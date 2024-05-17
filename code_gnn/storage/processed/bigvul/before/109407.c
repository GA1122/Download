void AsyncFileSystemChromium::createWriter(AsyncFileWriterClient* client, const KURL& path, PassOwnPtr<AsyncFileSystemCallbacks> callbacks)
{
    m_webFileSystem->readMetadata(path, new FileWriterHelperCallbacks(client, path, m_webFileSystem, callbacks));
}
