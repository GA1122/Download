DEFINE_TRACE(ResourceFetcher) {
  visitor->Trace(context_);
  visitor->Trace(archive_);
  visitor->Trace(loaders_);
  visitor->Trace(non_blocking_loaders_);
  visitor->Trace(document_resources_);
  visitor->Trace(preloads_);
  visitor->Trace(resource_timing_info_map_);
}
