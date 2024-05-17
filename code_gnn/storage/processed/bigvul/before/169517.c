ui::AXNode* FindNodeWithChildTreeId(ui::AXNode* node, int child_tree_id) {
  if (child_tree_id == node->data().GetIntAttribute(ui::AX_ATTR_CHILD_TREE_ID))
    return node;

  for (int i = 0; i < node->child_count(); ++i) {
    ui::AXNode* result =
        FindNodeWithChildTreeId(node->ChildAtIndex(i), child_tree_id);
    if (result)
      return result;
  }

  return nullptr;
}
