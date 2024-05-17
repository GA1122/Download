static inline bool isLocalHost(WKStringRef host)
{
    return WKStringIsEqualToUTF8CString(host, "127.0.0.1") || WKStringIsEqualToUTF8CString(host, "localhost");
}
