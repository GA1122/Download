  bool IsRemovedNode(const AXNode* node) {
    return removed_node_ids.find(node->id()) != removed_node_ids.end();
  }
