static void cachedResourcesForDocument(Document* document, Vector<Resource*>& result)
{
    const ResourceFetcher::DocumentResourceMap& allResources = document->fetcher()->allResources();
    ResourceFetcher::DocumentResourceMap::const_iterator end = allResources.end();
    for (ResourceFetcher::DocumentResourceMap::const_iterator it = allResources.begin(); it != end; ++it) {
        Resource* cachedResource = it->value.get();

        switch (cachedResource->type()) {
        case Resource::Image:
            if (toImageResource(cachedResource)->stillNeedsLoad())
                continue;
            break;
        case Resource::Font:
            if (toFontResource(cachedResource)->stillNeedsLoad())
                continue;
            break;
        default:
            break;
        }

        result.append(cachedResource);
    }
}
