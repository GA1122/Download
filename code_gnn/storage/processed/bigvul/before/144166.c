bool LockContentsView::AcceleratorPressed(const ui::Accelerator& accelerator) {
  auto entry = accel_map_.find(accelerator);
  if (entry == accel_map_.end())
    return false;

  PerformAction(entry->second);
  return true;
}
