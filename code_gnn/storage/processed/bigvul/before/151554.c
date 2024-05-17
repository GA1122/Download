DEFINE_TRACE(ResourceFetcher) {
  visitor->Trace(context_);
  visitor->Trace(scheduler_);
  visitor->Trace(archive_);
  visitor->Trace(loaders_);
  visitor->Trace(non_blocking_loaders_);
  visitor->Trace(cached_resources_map_);
  visitor->Trace(document_resources_);
  visitor->Trace(resources_from_previous_fetcher_);
  visitor->Trace(preloads_);
  visitor->Trace(matched_preloads_);
  visitor->Trace(resource_timing_info_map_);
}
