void InspectorResourceAgent::didReceiveCORSRedirectResponse(unsigned long identifier, DocumentLoader* loader, const ResourceResponse& response, ResourceLoader* resourceLoader)
{
    didReceiveResourceResponse(identifier, loader, response, resourceLoader);
    didFinishLoading(identifier, loader, 0);
}
