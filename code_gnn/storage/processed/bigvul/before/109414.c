void AsyncFileSystemChromium::directoryExists(const KURL& path, PassOwnPtr<AsyncFileSystemCallbacks> callbacks)
{
    m_webFileSystem->directoryExists(path, new WebKit::WebFileSystemCallbacksImpl(callbacks));
}
