void AsyncFileSystemChromium::createSnapshotFileAndReadMetadata(const KURL& path, PassOwnPtr<AsyncFileSystemCallbacks> callbacks)
{
    m_webFileSystem->createSnapshotFileAndReadMetadata(path, new WebKit::WebFileSystemCallbacksImpl(callbacks));
}
