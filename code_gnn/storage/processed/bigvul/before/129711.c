ResourcePtr<RawResource> ResourceFetcher::fetchTextTrack(FetchRequest& request)
{
    return toRawResource(requestResource(Resource::TextTrack, request));
}
