LifecycleUnit::SortKey TestLifecycleUnit::GetSortKey() const {
  return SortKey(last_focused_time_);
}
