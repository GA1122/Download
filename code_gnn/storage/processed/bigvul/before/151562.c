void ResourceFetcher::HoldResourcesFromPreviousFetcher(
    ResourceFetcher* old_fetcher) {
  DCHECK(resources_from_previous_fetcher_.IsEmpty());
  for (Resource* resource : old_fetcher->document_resources_) {
    if (GetMemoryCache()->Contains(resource))
      resources_from_previous_fetcher_.insert(resource);
  }
}
