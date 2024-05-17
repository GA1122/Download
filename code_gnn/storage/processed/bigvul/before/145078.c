    ResourcePtr<Resource> resourceFromResourceResponse(ResourceResponse response, Resource::Type type = Resource::Raw)
    {
        if (response.url().isNull())
            response.setURL(KURL(ParsedURLString, kResourceURL));
        ResourcePtr<Resource> resource =
            new Resource(ResourceRequest(response.url()), type);
        resource->setResponse(response);
        memoryCache()->add(resource.get());

        return resource;
    }
