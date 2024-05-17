AXNode* AXTree::GetFromId(int32_t id) const {
  auto iter = id_map_.find(id);
  return iter != id_map_.end() ? iter->second : nullptr;
}
