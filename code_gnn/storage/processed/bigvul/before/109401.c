AsyncFileSystemChromium::AsyncFileSystemChromium()
    : m_webFileSystem(WebKit::Platform::current()->fileSystem())
{
    ASSERT(m_webFileSystem);
}
