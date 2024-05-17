void AsyncFileSystemChromium::readDirectory(const KURL& path, PassOwnPtr<AsyncFileSystemCallbacks> callbacks)
{
    m_webFileSystem->readDirectory(path, new WebKit::WebFileSystemCallbacksImpl(callbacks));
}
