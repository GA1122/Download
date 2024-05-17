void Layer::SetFilters(const FilterOperations& filters) {
  DCHECK(IsPropertyChangeAllowed());
  if (filters_ == filters)
    return;
  filters_ = filters;
  SetNeedsCommit();
  SetNeedsFilterContextIfNeeded();
}
