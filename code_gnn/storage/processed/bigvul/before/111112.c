const BlackBerry::Platform::String& WebPagePrivate::defaultUserAgent()
{
    static BlackBerry::Platform::String* defaultUserAgent = 0;
    if (!defaultUserAgent) {
        BlackBerry::Platform::DeviceInfo* info = BlackBerry::Platform::DeviceInfo::instance();
        char uaBuffer[256];
        int uaSize = snprintf(uaBuffer, 256, "Mozilla/5.0 (%s) AppleWebKit/%d.%d+ (KHTML, like Gecko) Version/%s %sSafari/%d.%d+",
            info->family(), WEBKIT_MAJOR_VERSION, WEBKIT_MINOR_VERSION, info->osVersion(),
            info->isMobile() ? "Mobile " : "", WEBKIT_MAJOR_VERSION, WEBKIT_MINOR_VERSION);

        if (uaSize <= 0 || uaSize >= 256)
            BLACKBERRY_CRASH();

        defaultUserAgent = new BlackBerry::Platform::String(uaBuffer, uaSize);
    }

    return *defaultUserAgent;
}
