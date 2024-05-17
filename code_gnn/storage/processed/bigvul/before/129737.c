static Resource* resourceFromDataURIRequest(const ResourceRequest& request, const ResourceLoaderOptions& resourceOptions)
{
    const KURL& url = request.url();
    ASSERT(url.protocolIsData());

    blink::WebString mimetype;
    blink::WebString charset;
    RefPtr<SharedBuffer> data = PassRefPtr<SharedBuffer>(blink::Platform::current()->parseDataURL(url, mimetype, charset));
    if (!data)
        return 0;
    ResourceResponse response(url, mimetype, data->size(), charset, String());

    Resource* resource = createResource(Resource::Image, request, charset);
    resource->setOptions(resourceOptions);
    resource->responseReceived(response);
    if (data->size())
        resource->setResourceBuffer(data);
    resource->finish();
    return resource;
}
