    ResourcePtr<Resource> fetch()
    {
        FetchRequest fetchRequest(ResourceRequest(KURL(ParsedURLString, kResourceURL)), FetchInitiatorInfo());
        return RawResource::fetchSynchronously(fetchRequest, fetcher());
    }
