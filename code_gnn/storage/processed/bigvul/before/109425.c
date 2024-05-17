BlobOriginCache::BlobOriginCache()
{
    SecurityOrigin::setCache(this);
}
