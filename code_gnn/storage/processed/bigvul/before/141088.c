ComputedAccessibleNode* Document::GetOrCreateComputedAccessibleNode(
    AXID ax_id,
    WebComputedAXTree* tree) {
  if (computed_node_mapping_.find(ax_id) == computed_node_mapping_.end()) {
    ComputedAccessibleNode* node =
        ComputedAccessibleNode::Create(ax_id, tree, this);
    computed_node_mapping_.insert(ax_id, node);
  }
  return computed_node_mapping_.at(ax_id);
}
