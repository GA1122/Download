bool SVGLayoutSupport::hasFilterResource(const LayoutObject& object)
{
    SVGResources* resources = SVGResourcesCache::cachedResourcesForLayoutObject(&object);
    return resources && resources->filter();
}
