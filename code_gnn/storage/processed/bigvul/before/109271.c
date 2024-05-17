static Vector<Resource*> cachedResourcesForFrame(Frame* frame)
{
    Vector<Resource*> result;

    const ResourceFetcher::DocumentResourceMap& allResources = frame->document()->fetcher()->allResources();
    ResourceFetcher::DocumentResourceMap::const_iterator end = allResources.end();
    for (ResourceFetcher::DocumentResourceMap::const_iterator it = allResources.begin(); it != end; ++it) {
        Resource* cachedResource = it->value.get();

        switch (cachedResource->type()) {
        case Resource::Image:
            if (static_cast<ImageResource*>(cachedResource)->stillNeedsLoad())
                continue;
            break;
        case Resource::Font:
            if (static_cast<FontResource*>(cachedResource)->stillNeedsLoad())
                continue;
            break;
        default:
            break;
        }

        result.append(cachedResource);
    }

    return result;
}
