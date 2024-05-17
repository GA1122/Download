bool XMLHttpRequest::isAllowedHTTPMethod(const String& method)
{
    return !equalIgnoringCase(method, "TRACE")
        && !equalIgnoringCase(method, "TRACK")
        && !equalIgnoringCase(method, "CONNECT");
}
