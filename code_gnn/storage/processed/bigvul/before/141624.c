int32_t AXTree::GetNextNegativeInternalNodeId() {
  int32_t return_value = next_negative_internal_node_id_;
  next_negative_internal_node_id_--;
  if (next_negative_internal_node_id_ > 0)
    next_negative_internal_node_id_ = -1;
  return return_value;
}
