size_t PropertyTreeState::CacheMemoryUsageInBytes() const {
  return Clip()->CacheMemoryUsageInBytes() +
         Transform()->CacheMemoryUsageInBytes();
}
