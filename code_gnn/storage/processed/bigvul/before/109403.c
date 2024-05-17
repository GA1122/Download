void AsyncFileSystemChromium::copy(const KURL& sourcePath, const KURL& destinationPath, PassOwnPtr<AsyncFileSystemCallbacks> callbacks)
{
    m_webFileSystem->copy(sourcePath, destinationPath, new WebKit::WebFileSystemCallbacksImpl(callbacks));
}
