void XMLHttpRequest::open(const String& method, const KURL& url, bool async, const String& user, const String& password, ExceptionState& es)
{
    KURL urlWithCredentials(url);
    urlWithCredentials.setUser(user);
    urlWithCredentials.setPass(password);

    open(method, urlWithCredentials, async, es);
}
