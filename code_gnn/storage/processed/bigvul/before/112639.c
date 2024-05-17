static void cancelAll(const ResourceLoaderSet& loaders)
{
    Vector<RefPtr<ResourceLoader> > loadersCopy;
    copyToVector(loaders, loadersCopy);
    size_t size = loadersCopy.size();
    for (size_t i = 0; i < size; ++i)
        loadersCopy[i]->cancel();
}
