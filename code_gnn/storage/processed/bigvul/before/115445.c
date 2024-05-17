static inline WTF::String pathSuitableForTestResult(WKURLRef fileUrl)
{
    if (!fileUrl)
        return String();

    WKRetainPtr<WKStringRef> schemeString = adoptWK(WKURLCopyScheme(fileUrl));
    if (!isLocalFileScheme(schemeString.get()))
        return toWTFString(adoptWK(WKURLCopyString(fileUrl)));

    String pathString = toWTFString(adoptWK(WKURLCopyPath(fileUrl)));
    StringBuilder stringBuilder;

    const size_t indexBaseName = pathString.reverseFind(divider);
    if (indexBaseName != notFound) {
        const size_t indexDirName = pathString.reverseFind(divider, indexBaseName - 1);
        if (indexDirName != notFound)
            stringBuilder.append(pathString.substring(indexDirName + 1, indexBaseName - indexDirName - 1));
        stringBuilder.append(divider);
        stringBuilder.append(pathString.substring(indexBaseName + 1));  
    } else {
        stringBuilder.append(divider);
        stringBuilder.append(pathString);  
    }

    return stringBuilder.toString();
}
