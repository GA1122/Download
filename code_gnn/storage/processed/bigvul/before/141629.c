int32_t AXTree::GetSetSize(const AXNode& node, const AXNode* ordered_set) {
  if (ordered_set_info_map_.find(node.id()) == ordered_set_info_map_.end())
    ComputeSetSizePosInSetAndCache(node, ordered_set);
  return ordered_set_info_map_[node.id()].set_size;
}
