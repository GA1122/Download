void AXTree::DestroyNodeAndSubtree(AXNode* node,
                                   AXTreeUpdateState* update_state) {
  AXNodeData empty_data;
  empty_data.id = node->id();
  UpdateReverseRelations(node, empty_data);

  const auto& table_info_entry = table_info_map_.find(node->id());
  if (table_info_entry != table_info_map_.end()) {
    delete table_info_entry->second;
    table_info_map_.erase(node->id());
  }

  for (AXTreeObserver& observer : observers_) {
    if (!update_state || !update_state->IsChangedNode(node))
      observer.OnNodeWillBeDeleted(this, node);
    else
      observer.OnNodeWillBeReparented(this, node);
  }
  id_map_.erase(node->id());
  for (int i = 0; i < node->child_count(); ++i)
    DestroyNodeAndSubtree(node->ChildAtIndex(i), update_state);
  if (update_state) {
    update_state->pending_nodes.erase(node);
    update_state->removed_node_ids.insert(node->id());
  }

  if (update_state && update_state->IsChangedNode(node)) {
    update_state->reparented_node_id_to_data.insert(
        std::make_pair(node->id(), node->TakeData()));
  }
  node->Destroy();
}
