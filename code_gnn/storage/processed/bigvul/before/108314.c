static bool shouldLoadAsEmptyDocument(const KURL& url)
{
#if PLATFORM(TORCHMOBILE)
    return url.isEmpty() || (url.protocolIs("about") && equalIgnoringRef(url, blankURL()));
#else 
    return url.isEmpty() || url.protocolIs("about");
#endif
}
