void AsyncFileSystemChromium::readMetadata(const KURL& path, PassOwnPtr<AsyncFileSystemCallbacks> callbacks)
{
    m_webFileSystem->readMetadata(path, new WebKit::WebFileSystemCallbacksImpl(callbacks));
}
