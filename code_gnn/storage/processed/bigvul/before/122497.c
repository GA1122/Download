void InspectorResourceAgent::didReceiveCORSRedirectResponse(LocalFrame* frame, unsigned long identifier, DocumentLoader* loader, const ResourceResponse& response, ResourceLoader* resourceLoader)
{
    didReceiveResourceResponse(frame, identifier, loader, response, resourceLoader);
    didFinishLoading(identifier, loader, 0, blink::WebURLLoaderClient::kUnknownEncodedDataLength);
}
