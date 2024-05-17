size_t ClipPaintPropertyNode::CacheMemoryUsageInBytes() const {
  size_t total_bytes = sizeof(*this);
  if (geometry_mapper_clip_cache_)
    total_bytes += sizeof(*geometry_mapper_clip_cache_);
  if (Parent())
    total_bytes += Parent()->CacheMemoryUsageInBytes();
  return total_bytes;
}
