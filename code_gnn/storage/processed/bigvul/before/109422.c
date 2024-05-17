 URLRegistry& BlobURLRegistry::registry()
{
    DEFINE_STATIC_LOCAL(BlobURLRegistry, instance, ());
    return instance;
}
