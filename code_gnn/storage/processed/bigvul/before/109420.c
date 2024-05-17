void AsyncFileSystemChromium::removeRecursively(const KURL& path, PassOwnPtr<AsyncFileSystemCallbacks> callbacks)
{
    m_webFileSystem->removeRecursively(path, new WebKit::WebFileSystemCallbacksImpl(callbacks));
}
