size_t PaintController::FindOutOfOrderCachedItemForward(
    const DisplayItem::Id& id) {
  for (size_t i = next_item_to_index_;
       i < current_paint_artifact_.GetDisplayItemList().size(); ++i) {
    const DisplayItem& item = current_paint_artifact_.GetDisplayItemList()[i];
    if (item.IsTombstone())
      continue;
    if (id == item.GetId()) {
#if DCHECK_IS_ON()
      ++num_sequential_matches_;
#endif
      return i;
    }
    if (item.IsCacheable()) {
#if DCHECK_IS_ON()
      ++num_indexed_items_;
#endif
      AddToIndicesByClientMap(item.Client(), i, out_of_order_item_indices_);
    }
  }

  if (RuntimeEnabledFeatures::PaintUnderInvalidationCheckingEnabled()) {
#if DCHECK_IS_ON()
    ShowDebugData();
#endif
    LOG(WARNING) << "Can't find cached display item: " << id.client.DebugName()
                 << " " << id.ToString();
  }
  return kNotFound;
}
