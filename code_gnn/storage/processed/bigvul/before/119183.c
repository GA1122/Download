void XMLHttpRequest::open(const String& method, const KURL& url, ExceptionState& es)
{
    open(method, url, true, es);
}
