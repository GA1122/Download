ResourceLoader* ResourceLoader::Create(ResourceFetcher* fetcher,
                                       ResourceLoadScheduler* scheduler,
                                       Resource* resource) {
  return new ResourceLoader(fetcher, scheduler, resource);
}
