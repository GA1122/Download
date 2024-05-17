void WebPage::loadFile(const BlackBerry::Platform::String& path, const BlackBerry::Platform::String& overrideContentType)
{
    BlackBerry::Platform::String fileUrl(path);
    if (fileUrl.startsWith("/"))
        fileUrl = BlackBerry::Platform::String("file://", 7) + fileUrl;
    else if (!fileUrl.startsWith("file:///"))
        return;

    d->load(fileUrl, BlackBerry::Platform::String::emptyString(), BlackBerry::Platform::String("GET", 3), Platform::NetworkRequest::UseProtocolCachePolicy, 0, 0, 0, 0, false, false, false, overrideContentType.c_str());
}
