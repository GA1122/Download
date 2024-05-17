AXTableInfo* AXTree::GetTableInfo(const AXNode* const_table_node) const {
  AXNode* table_node = const_cast<AXNode*>(const_table_node);
  AXTree* tree = const_cast<AXTree*>(this);

  DCHECK(table_node);
  const auto& cached = table_info_map_.find(table_node->id());
  if (cached != table_info_map_.end()) {
    AXTableInfo* table_info = cached->second;
    if (!table_info->valid()) {
      bool success = table_info->Update();
      if (!success) {
        delete table_info;
        table_info = nullptr;
        table_info_map_.erase(table_node->id());
      }
      for (AXTreeObserver& observer : observers_)
        observer.OnNodeChanged(tree, table_node);
    }
    return table_info;
  }

  AXTableInfo* table_info = AXTableInfo::Create(tree, table_node);
  if (!table_info)
    return nullptr;

  table_info_map_[table_node->id()] = table_info;
  for (AXTreeObserver& observer : observers_)
    observer.OnNodeChanged(tree, table_node);

  return table_info;
}
