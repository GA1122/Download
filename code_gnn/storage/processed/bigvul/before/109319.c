void InspectorPageAgent::resourceContent(ErrorString* errorString, Frame* frame, const KURL& url, String* result, bool* base64Encoded)
{
    DocumentLoader* loader = assertDocumentLoader(errorString, frame);
    if (!loader)
        return;
    if (!cachedResourceContent(cachedResource(frame, url), result, base64Encoded))
        *errorString = "No resource with given URL found";
}
