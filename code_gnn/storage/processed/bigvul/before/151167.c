void InspectorNetworkAgent::DidReceiveCORSRedirectResponse(
    unsigned long identifier,
    DocumentLoader* loader,
    const ResourceResponse& response,
    Resource* resource) {
  DidReceiveResourceResponse(identifier, loader, response, resource);
  DidFinishLoading(identifier, loader, 0,
                   WebURLLoaderClient::kUnknownEncodedDataLength, 0);
}
