HarfBuzzRunCache& harfBuzzRunCache()
{
    DEFINE_STATIC_LOCAL(HarfBuzzRunCache, globalHarfBuzzRunCache, ());
    return globalHarfBuzzRunCache;
}
