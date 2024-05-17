    ResourcePtr<Resource> fetchImage()
    {
        FetchRequest fetchRequest(ResourceRequest(KURL(ParsedURLString, kResourceURL)), FetchInitiatorInfo());
        return ImageResource::fetch(fetchRequest, fetcher());
    }
