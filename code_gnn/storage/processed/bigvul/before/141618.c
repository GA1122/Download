AXNode* AXTree::CreateNode(AXNode* parent,
                           int32_t id,
                           int32_t index_in_parent,
                           AXTreeUpdateState* update_state) {
  AXNode* new_node = new AXNode(this, parent, id, index_in_parent);
  id_map_[new_node->id()] = new_node;
  for (AXTreeObserver& observer : observers_) {
    if (update_state->IsChangedNode(new_node) &&
        !update_state->IsRemovedNode(new_node))
      observer.OnNodeCreated(this, new_node);
    else
      observer.OnNodeReparented(this, new_node);
  }
  return new_node;
}
