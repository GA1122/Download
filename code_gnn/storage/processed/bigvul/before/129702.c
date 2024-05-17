ResourcePtr<FontResource> ResourceFetcher::fetchFont(FetchRequest& request)
{
    return toFontResource(requestResource(Resource::Font, request));
}
