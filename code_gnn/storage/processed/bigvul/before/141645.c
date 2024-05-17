bool AXTree::UpdateNode(const AXNodeData& src,
                        bool is_new_root,
                        AXTreeUpdateState* update_state) {

  AXNode* node = GetFromId(src.id);
  if (node) {
    update_state->pending_nodes.erase(node);

    if (!update_state->IsNewNode(node) ||
        update_state->IsReparentedNode(node)) {
      auto it = update_state->reparented_node_id_to_data.find(node->id());
      if (it != update_state->reparented_node_id_to_data.end())
        CallNodeChangeCallbacks(node, it->second, src);
      else
        CallNodeChangeCallbacks(node, node->data(), src);
    }
    UpdateReverseRelations(node, src);
    node->SetData(src);
  } else {
    if (!is_new_root) {
      error_ = base::StringPrintf(
          "%d is not in the tree and not the new root", src.id);
      return false;
    }

    update_state->new_root = CreateNode(nullptr, src.id, 0, update_state);
    node = update_state->new_root;
    update_state->new_nodes.insert(node);
    UpdateReverseRelations(node, src);
    node->SetData(src);
  }

  for (AXTreeObserver& observer : observers_)
    observer.OnNodeChanged(this, node);

  if (!DeleteOldChildren(node, src.child_ids, update_state)) {
    if (update_state->new_root) {
      AXNode* old_root = root_;
      root_ = nullptr;

      DestroySubtree(old_root, update_state);

      if (update_state->removed_node_ids.find(src.id) ==
              update_state->removed_node_ids.end() &&
          update_state->new_nodes.find(node) != update_state->new_nodes.end()) {
        DestroySubtree(node, update_state);
      }
    }
    return false;
  }

  std::vector<AXNode*> new_children;
  bool success = CreateNewChildVector(
      node, src.child_ids, &new_children, update_state);
  node->SwapChildren(new_children);

  if (is_new_root) {
    AXNode* old_root = root_;
    root_ = node;
    if (old_root && old_root != node)
      DestroySubtree(old_root, update_state);
  }

  return success;
}
