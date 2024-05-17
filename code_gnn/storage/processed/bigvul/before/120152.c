void Layer::SetBackgroundFilters(const FilterOperations& filters) {
  DCHECK(IsPropertyChangeAllowed());
  if (background_filters_ == filters)
    return;
  background_filters_ = filters;
  SetNeedsCommit();
  SetNeedsFilterContextIfNeeded();
}
