void DocumentThreadableLoader::loadResourceSynchronously(Document& document, const ResourceRequest& request, ThreadableLoaderClient& client, const ThreadableLoaderOptions& options, const ResourceLoaderOptions& resourceLoaderOptions)
{
    OwnPtr<DocumentThreadableLoader> loader = adoptPtr(new DocumentThreadableLoader(document, &client, LoadSynchronously, options, resourceLoaderOptions));
    loader->start(request);
}
