void ResourceFetcher::RequestLoadStarted(unsigned long identifier,
                                         Resource* resource,
                                         const FetchParameters& params,
                                         RevalidationPolicy policy,
                                         bool is_static_data) {
  KURL url = MemoryCache::RemoveFragmentIdentifierIfNeeded(params.Url());
  if (policy == kUse && resource->GetStatus() == ResourceStatus::kCached &&
      !cached_resources_map_.Contains(url)) {
    DidLoadResourceFromMemoryCache(identifier, resource,
                                   params.GetResourceRequest());
  }

  if (is_static_data)
    return;

  if (policy == kUse && !resource->StillNeedsLoad() &&
      !cached_resources_map_.Contains(url)) {
    RefPtr<ResourceTimingInfo> info = ResourceTimingInfo::Create(
        params.Options().initiator_info.name, MonotonicallyIncreasingTime(),
        resource->GetType() == Resource::kMainResource);
    PopulateTimingInfo(info.get(), resource);
    info->ClearLoadTimings();
    info->SetLoadFinishTime(info->InitialTime());
    scheduled_resource_timing_reports_.push_back(std::move(info));
    if (!resource_timing_report_timer_.IsActive())
      resource_timing_report_timer_.StartOneShot(0, BLINK_FROM_HERE);
  }
}
