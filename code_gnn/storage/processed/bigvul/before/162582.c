bool Resource::ShouldBlockLoadEvent() const {
  return !link_preload_ && IsLoadEventBlockingResourceType();
}
