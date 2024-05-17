ResourcePtr<RawResource> ResourceFetcher::fetchImport(FetchRequest& request)
{
    return toRawResource(requestResource(Resource::ImportResource, request));
}
