static WTF::String lastFileURLPathComponent(const WTF::String& path)
{
    size_t pos = path.find("file://");
    ASSERT(WTF::notFound != pos);

    WTF::String tmpPath = path.substring(pos + 7);
    if (tmpPath.length() < 2)  
        return tmpPath;

    if (tmpPath[tmpPath.length() - 1] == '/')
        tmpPath.remove(tmpPath.length() - 1);

    pos = tmpPath.reverseFind('/');
    if (WTF::notFound != pos)
        return tmpPath.substring(pos + 1);

    return tmpPath;
}
