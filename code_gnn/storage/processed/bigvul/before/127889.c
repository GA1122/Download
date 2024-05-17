void DocumentThreadableLoader::cancel()
{
    cancelWithError(ResourceError());
}
