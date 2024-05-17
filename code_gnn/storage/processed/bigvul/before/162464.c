void ImageResource::FlagAsUserAgentResource() {
  if (is_referenced_from_ua_stylesheet_)
    return;

  InstanceCounters::IncrementCounter(InstanceCounters::kUACSSResourceCounter);
  is_referenced_from_ua_stylesheet_ = true;
}
