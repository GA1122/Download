const std::set<AXTreeID> AXTree::GetAllChildTreeIds() const {
  std::set<AXTreeID> result;
  for (auto entry : child_tree_id_reverse_map_)
    result.insert(entry.first);
  return result;
}
