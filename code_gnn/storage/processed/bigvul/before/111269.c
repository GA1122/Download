static KURL parseUrl(const String& url)
{
    String urlString(url);
    KURL kurl = KURL(KURL(), urlString);
    if (kurl.protocol().isEmpty()) {
        urlString.insert("http://", 0);
        kurl = KURL(KURL(), urlString);
    }

    return kurl;
}
