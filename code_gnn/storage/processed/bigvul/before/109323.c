void InspectorPageAgent::searchInResource(ErrorString*, const String& frameId, const String& url, const String& query, const bool* const optionalCaseSensitive, const bool* const optionalIsRegex, RefPtr<TypeBuilder::Array<TypeBuilder::Page::SearchMatch> >& results)
{
    results = TypeBuilder::Array<TypeBuilder::Page::SearchMatch>::create();

    bool isRegex = optionalIsRegex ? *optionalIsRegex : false;
    bool caseSensitive = optionalCaseSensitive ? *optionalCaseSensitive : false;

    Frame* frame = frameForId(frameId);
    KURL kurl(ParsedURLString, url);

    FrameLoader* frameLoader = frame ? &frame->loader() : 0;
    DocumentLoader* loader = frameLoader ? frameLoader->documentLoader() : 0;
    if (!loader)
        return;

    String content;
    bool success = false;
    Resource* resource = cachedResource(frame, kurl);
    if (resource)
        success = textContentForResource(resource, &content);

    if (!success)
        return;

    results = ContentSearchUtils::searchInTextByLines(content, query, caseSensitive, isRegex);
}
