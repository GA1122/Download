ResourcePtr<RawResource> ResourceFetcher::fetchMedia(FetchRequest& request)
{
    return toRawResource(requestResource(Resource::Media, request));
}
