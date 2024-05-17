void SVGLayoutSupport::layoutResourcesIfNeeded(const LayoutObject* object)
{
    ASSERT(object);

    SVGResources* resources = SVGResourcesCache::cachedResourcesForLayoutObject(object);
    if (resources)
        resources->layoutIfNeeded();
}
