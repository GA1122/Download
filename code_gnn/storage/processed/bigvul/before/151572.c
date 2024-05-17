void ResourceFetcher::RecordResourceTimingOnRedirect(
    Resource* resource,
    const ResourceResponse& redirect_response,
    bool cross_origin) {
  ResourceTimingInfoMap::iterator it = resource_timing_info_map_.find(resource);
  if (it != resource_timing_info_map_.end()) {
    it->value->AddRedirect(redirect_response, cross_origin);
  }

  if (resource->GetType() == Resource::kMainResource) {
    DCHECK(navigation_timing_info_);
    navigation_timing_info_->AddRedirect(redirect_response, cross_origin);
  }
}
