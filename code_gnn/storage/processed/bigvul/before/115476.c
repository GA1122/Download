static inline WTF::String urlSuitableForTestResult(WKURLRef fileUrl)
{
    if (!fileUrl)
        return String();

    WKRetainPtr<WKStringRef> schemeString = adoptWK(WKURLCopyScheme(fileUrl));
    if (!isLocalFileScheme(schemeString.get()))
        return toWTFString(adoptWK(WKURLCopyString(fileUrl)));

    WTF::String urlString = toWTFString(adoptWK(WKURLCopyString(fileUrl)));
    const size_t indexBaseName = urlString.reverseFind(divider);

    return (indexBaseName == notFound) ? urlString : urlString.substring(indexBaseName + 1);
}
