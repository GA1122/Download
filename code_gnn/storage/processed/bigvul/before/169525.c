TreeCache* AutomationInternalCustomBindings::GetTreeCacheFromTreeID(
    int tree_id) {
  const auto iter = tree_id_to_tree_cache_map_.find(tree_id);
  if (iter == tree_id_to_tree_cache_map_.end())
    return nullptr;

  return iter->second;
}
