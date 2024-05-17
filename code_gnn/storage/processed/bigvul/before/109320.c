String InspectorPageAgent::resourceSourceMapURL(const String& url)
{
    DEFINE_STATIC_LOCAL(String, sourceMapHttpHeader, ("SourceMap"));
    DEFINE_STATIC_LOCAL(String, deprecatedSourceMapHttpHeader, ("X-SourceMap"));
    if (url.isEmpty())
        return String();
    Frame* frame = mainFrame();
    if (!frame)
        return String();
    Resource* resource = cachedResource(frame, KURL(ParsedURLString, url));
    if (!resource)
        return String();
    String deprecatedHeaderSourceMapURL = resource->response().httpHeaderField(deprecatedSourceMapHttpHeader);
    if (!deprecatedHeaderSourceMapURL.isEmpty()) {
        return deprecatedHeaderSourceMapURL;
    }
    return resource->response().httpHeaderField(sourceMapHttpHeader);
}
