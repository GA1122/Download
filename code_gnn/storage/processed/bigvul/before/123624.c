const AtomicString& InspectorPageAgent::resourceSourceMapURL(const String& url)
{
    DEFINE_STATIC_LOCAL(const AtomicString, sourceMapHttpHeader, ("SourceMap", AtomicString::ConstructFromLiteral));
    DEFINE_STATIC_LOCAL(const AtomicString, deprecatedSourceMapHttpHeader, ("X-SourceMap", AtomicString::ConstructFromLiteral));
    if (url.isEmpty())
        return nullAtom;
    Frame* frame = mainFrame();
    if (!frame)
        return nullAtom;
    Resource* resource = cachedResource(frame, KURL(ParsedURLString, url));
    if (!resource)
        return nullAtom;
    const AtomicString& deprecatedHeaderSourceMapURL = resource->response().httpHeaderField(deprecatedSourceMapHttpHeader);
    if (!deprecatedHeaderSourceMapURL.isEmpty()) {
        return deprecatedHeaderSourceMapURL;
    }
    return resource->response().httpHeaderField(sourceMapHttpHeader);
}
