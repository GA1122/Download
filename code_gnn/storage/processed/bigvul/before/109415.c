void AsyncFileSystemChromium::fileExists(const KURL& path, PassOwnPtr<AsyncFileSystemCallbacks> callbacks)
{
    m_webFileSystem->fileExists(path, new WebKit::WebFileSystemCallbacksImpl(callbacks));
}
