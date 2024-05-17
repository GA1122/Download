static unsigned long long exceededDatabaseQuota(WKPageRef, WKFrameRef, WKSecurityOriginRef, WKStringRef, WKStringRef, unsigned long long, unsigned long long, unsigned long long, unsigned long long, const void*)
{
    static const unsigned long long defaultQuota = 5 * 1024 * 1024;
    return defaultQuota;
}
