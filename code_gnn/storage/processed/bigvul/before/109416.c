void AsyncFileSystemChromium::move(const KURL& sourcePath, const KURL& destinationPath, PassOwnPtr<AsyncFileSystemCallbacks> callbacks)
{
    m_webFileSystem->move(sourcePath, destinationPath, new WebKit::WebFileSystemCallbacksImpl(callbacks));
}
