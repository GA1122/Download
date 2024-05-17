size_t PaintController::FindMatchingItemFromIndex(
    const DisplayItem::Id& id,
    const IndicesByClientMap& display_item_indices_by_client,
    const DisplayItemList& list) {
  IndicesByClientMap::const_iterator it =
      display_item_indices_by_client.find(&id.client);
  if (it == display_item_indices_by_client.end())
    return kNotFound;

  const Vector<size_t>& indices = it->value;
  for (size_t index : indices) {
    const DisplayItem& existing_item = list[index];
    if (existing_item.IsTombstone())
      continue;
    DCHECK(existing_item.Client() == id.client);
    if (id == existing_item.GetId())
      return index;
  }

  return kNotFound;
}
