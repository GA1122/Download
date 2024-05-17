size_t PaintController::FindCachedItem(const DisplayItem::Id& id) {
  DCHECK(ClientCacheIsValid(id.client));

  for (size_t i = next_item_to_match_;
       i < current_paint_artifact_.GetDisplayItemList().size(); ++i) {
    const DisplayItem& item = current_paint_artifact_.GetDisplayItemList()[i];
    if (item.IsTombstone())
      break;
    if (id == item.GetId()) {
#if DCHECK_IS_ON()
      ++num_sequential_matches_;
#endif
      return i;
    }
    if (item.IsCacheable())
      break;
  }

  size_t found_index =
      FindMatchingItemFromIndex(id, out_of_order_item_indices_,
                                current_paint_artifact_.GetDisplayItemList());
  if (found_index != kNotFound) {
#if DCHECK_IS_ON()
    ++num_out_of_order_matches_;
#endif
    return found_index;
  }

  return FindOutOfOrderCachedItemForward(id);
}
