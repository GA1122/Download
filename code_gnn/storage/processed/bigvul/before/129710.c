ResourcePtr<Resource> ResourceFetcher::fetchSynchronously(FetchRequest& request)
{
    ASSERT(document());
    request.mutableResourceRequest().setTimeoutInterval(10);
    ResourceLoaderOptions options(request.options());
    options.synchronousPolicy = RequestSynchronously;
    request.setOptions(options);
    return requestResource(Resource::Raw, request);
}
