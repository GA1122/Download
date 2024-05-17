void XMLHttpRequest::open(const String& method, const KURL& url, bool async, const String& user, ExceptionState& es)
{
    KURL urlWithCredentials(url);
    urlWithCredentials.setUser(user);

    open(method, urlWithCredentials, async, es);
}
