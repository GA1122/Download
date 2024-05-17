std::set<int32_t> AXTree::GetNodeIdsForChildTreeId(
    AXTreeID child_tree_id) const {
  const auto& result = child_tree_id_reverse_map_.find(child_tree_id);
  if (result != child_tree_id_reverse_map_.end())
    return result->second;
  return std::set<int32_t>();
}
