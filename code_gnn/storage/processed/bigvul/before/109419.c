void AsyncFileSystemChromium::remove(const KURL& path, PassOwnPtr<AsyncFileSystemCallbacks> callbacks)
{
    m_webFileSystem->remove(path, new WebKit::WebFileSystemCallbacksImpl(callbacks));
}
