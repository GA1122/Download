ResourceLoadPriority ResourceFetcher::ComputeLoadPriority(
    Resource::Type type,
    const ResourceRequest& resource_request,
    ResourcePriority::VisibilityStatus visibility,
    FetchRequest::DeferOption defer_option,
    bool is_speculative_preload,
    bool is_link_preload) {
  ResourceLoadPriority priority = TypeToPriority(type);

  if (visibility == ResourcePriority::kVisible)
    priority = kResourceLoadPriorityHigh;

  if (type == Resource::kImage && !is_link_preload)
    image_fetched_ = true;

  if (type == Resource::kFont && is_link_preload)
    priority = kResourceLoadPriorityHigh;

  if (FetchRequest::kIdleLoad == defer_option) {
    priority = kResourceLoadPriorityVeryLow;
  } else if (type == Resource::kScript) {
    if (FetchRequest::kLazyLoad == defer_option) {
      priority = kResourceLoadPriorityLow;
    } else if (is_speculative_preload && image_fetched_) {
      priority = kResourceLoadPriorityMedium;
    }
  } else if (FetchRequest::kLazyLoad == defer_option) {
    priority = kResourceLoadPriorityVeryLow;
  }

  return std::max(Context().ModifyPriorityForExperiments(priority),
                  resource_request.Priority());
}
