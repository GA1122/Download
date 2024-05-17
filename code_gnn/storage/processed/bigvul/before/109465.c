BlobRegistry& blobRegistry()
{
    ASSERT(isMainThread());
    DEFINE_STATIC_LOCAL(BlobRegistryProxy, instance, ());
    return instance;
}
