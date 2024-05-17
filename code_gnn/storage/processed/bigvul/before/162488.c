ImageResource::~ImageResource() {
  RESOURCE_LOADING_DVLOG(1) << "~ImageResource " << this;

  if (is_referenced_from_ua_stylesheet_)
    InstanceCounters::DecrementCounter(InstanceCounters::kUACSSResourceCounter);
}
