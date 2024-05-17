static PassRefPtr<TypeBuilder::Network::Response> buildObjectForResourceResponse(const ResourceResponse& response, DocumentLoader* loader)
{
    if (response.isNull())
        return nullptr;

    double status;
    String statusText;
    if (response.resourceLoadInfo() && response.resourceLoadInfo()->httpStatusCode) {
        status = response.resourceLoadInfo()->httpStatusCode;
        statusText = response.resourceLoadInfo()->httpStatusText;
    } else {
        status = response.httpStatusCode();
        statusText = response.httpStatusText();
    }
    RefPtr<JSONObject> headers;
    if (response.resourceLoadInfo())
        headers = buildObjectForHeaders(response.resourceLoadInfo()->responseHeaders);
    else
        headers = buildObjectForHeaders(response.httpHeaderFields());

    int64_t encodedDataLength = response.resourceLoadInfo() ? response.resourceLoadInfo()->encodedDataLength : -1;

    RefPtr<TypeBuilder::Network::Response> responseObject = TypeBuilder::Network::Response::create()
        .setUrl(urlWithoutFragment(response.url()).string())
        .setStatus(status)
        .setStatusText(statusText)
        .setHeaders(headers)
        .setMimeType(response.mimeType())
        .setConnectionReused(response.connectionReused())
        .setConnectionId(response.connectionID())
        .setEncodedDataLength(encodedDataLength);

    responseObject->setFromDiskCache(response.wasCached());
    if (response.resourceLoadTiming())
        responseObject->setTiming(buildObjectForTiming(*response.resourceLoadTiming(), loader));

    if (response.resourceLoadInfo()) {
        if (!response.resourceLoadInfo()->responseHeadersText.isEmpty())
            responseObject->setHeadersText(response.resourceLoadInfo()->responseHeadersText);

        responseObject->setRequestHeaders(buildObjectForHeaders(response.resourceLoadInfo()->requestHeaders));
        if (!response.resourceLoadInfo()->requestHeadersText.isEmpty())
            responseObject->setRequestHeadersText(response.resourceLoadInfo()->requestHeadersText);
    }

    AtomicString remoteIPAddress = response.remoteIPAddress();
    if (!remoteIPAddress.isEmpty()) {
        responseObject->setRemoteIPAddress(remoteIPAddress);
        responseObject->setRemotePort(response.remotePort());
    }

    return responseObject;
}