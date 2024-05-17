String XMLHttpRequest::uppercaseKnownHTTPMethod(const String& method)
{
    if (equalIgnoringCase(method, "COPY") || equalIgnoringCase(method, "DELETE") || equalIgnoringCase(method, "GET")
        || equalIgnoringCase(method, "HEAD") || equalIgnoringCase(method, "INDEX") || equalIgnoringCase(method, "LOCK")
        || equalIgnoringCase(method, "M-POST") || equalIgnoringCase(method, "MKCOL") || equalIgnoringCase(method, "MOVE")
        || equalIgnoringCase(method, "OPTIONS") || equalIgnoringCase(method, "POST") || equalIgnoringCase(method, "PROPFIND")
        || equalIgnoringCase(method, "PROPPATCH") || equalIgnoringCase(method, "PUT") || equalIgnoringCase(method, "UNLOCK")) {
        return method.upper();
    }
    return method;
}
