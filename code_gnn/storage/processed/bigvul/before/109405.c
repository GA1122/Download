void AsyncFileSystemChromium::createFile(const KURL& path, bool exclusive, PassOwnPtr<AsyncFileSystemCallbacks> callbacks)
{
    m_webFileSystem->createFile(path, exclusive, new WebKit::WebFileSystemCallbacksImpl(callbacks));
}
