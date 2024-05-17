void InspectorResourceAgent::loadResourceForFrontend(ErrorString* errorString, const String& frameId, const String& url, const RefPtr<JSONObject>* requestHeaders, PassRefPtr<LoadResourceForFrontendCallback> prpCallback)
{
    RefPtr<LoadResourceForFrontendCallback> callback = prpCallback;
    Frame* frame = m_pageAgent->assertFrame(errorString, frameId);
    if (!frame)
        return;

    Document* document = frame->document();
    if (!document) {
        *errorString = "No Document instance for the specified frame";
        return;
    }

    ResourceRequest request(url);
    request.setHTTPMethod("GET");
    request.setCachePolicy(ReloadIgnoringCacheData);
    if (requestHeaders) {
        for (JSONObject::iterator it = (*requestHeaders)->begin(); it != (*requestHeaders)->end(); ++it) {
            String value;
            bool success = it->value->asString(&value);
            if (!success) {
                *errorString = "Request header \"" + it->key + "\" value is not a string";
                return;
            }
            request.addHTTPHeaderField(it->key, value);
        }
    }

    ThreadableLoaderOptions options;
    options.allowCredentials = AllowStoredCredentials;
    options.crossOriginRequestPolicy = AllowCrossOriginRequests;
    options.sendLoadCallbacks = SendCallbacks;

    InspectorThreadableLoaderClient* inspectorThreadableLoaderClient = new InspectorThreadableLoaderClient(callback);
    RefPtr<DocumentThreadableLoader> loader = DocumentThreadableLoader::create(document, inspectorThreadableLoaderClient, request, options);
    if (!loader) {
        inspectorThreadableLoaderClient->didFailLoaderCreation();
        return;
    }
    loader->setDefersLoading(false);
    if (!callback->isActive())
        return;
    inspectorThreadableLoaderClient->setLoader(loader.release());
}
