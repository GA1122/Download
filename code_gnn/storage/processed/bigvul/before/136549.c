bool PaintController::UseCachedDrawingIfPossible(
    const DisplayItemClient& client,
    DisplayItem::Type type) {
  DCHECK(DisplayItem::IsDrawingType(type));

  if (DisplayItemConstructionIsDisabled())
    return false;

  if (!ClientCacheIsValid(client))
    return false;

  if (RuntimeEnabledFeatures::PaintUnderInvalidationCheckingEnabled() &&
      IsCheckingUnderInvalidation()) {
    return false;
  }

  size_t cached_item =
      FindCachedItem(DisplayItem::Id(client, type, current_fragment_));
  if (cached_item == kNotFound) {
    return false;
  }

  ++num_cached_new_items_;
  EnsureNewDisplayItemListInitialCapacity();
  current_paint_artifact_.GetDisplayItemList()[cached_item].UpdateVisualRect();
  if (!RuntimeEnabledFeatures::PaintUnderInvalidationCheckingEnabled())
    ProcessNewItem(MoveItemFromCurrentListToNewList(cached_item));

  next_item_to_match_ = cached_item + 1;
  if (next_item_to_match_ > next_item_to_index_)
    next_item_to_index_ = next_item_to_match_;

  if (RuntimeEnabledFeatures::PaintUnderInvalidationCheckingEnabled()) {
    if (!IsCheckingUnderInvalidation()) {
      under_invalidation_checking_begin_ = cached_item;
      under_invalidation_checking_end_ = cached_item + 1;
      under_invalidation_message_prefix_ = "";
    }
    return false;
  }

  return true;
}
