size_t PaintController::ApproximateUnsharedMemoryUsage() const {
  size_t memory_usage = sizeof(*this);

  memory_usage += current_paint_artifact_.ApproximateUnsharedMemoryUsage() -
                  sizeof(current_paint_artifact_);


  DCHECK(new_display_item_list_.IsEmpty());
  memory_usage += new_display_item_list_.MemoryUsageInBytes();

  memory_usage += current_cached_subsequences_.Capacity() *
                  sizeof(*current_cached_subsequences_.begin());
  DCHECK(new_cached_subsequences_.IsEmpty());
  memory_usage += new_cached_subsequences_.Capacity() *
                  sizeof(*new_cached_subsequences_.begin());

  return memory_usage;
}
