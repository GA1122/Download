PassOwnPtr<DocumentThreadableLoader> DocumentThreadableLoader::create(Document& document, ThreadableLoaderClient* client, const ThreadableLoaderOptions& options, const ResourceLoaderOptions& resourceLoaderOptions)
{
    return adoptPtr(new DocumentThreadableLoader(document, client, LoadAsynchronously, options, resourceLoaderOptions));
}
