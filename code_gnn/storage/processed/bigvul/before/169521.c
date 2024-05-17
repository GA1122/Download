ui::AXNode* AutomationInternalCustomBindings::GetParent(
    ui::AXNode* node,
    TreeCache** in_out_cache) {
  if (node->parent())
    return node->parent();

  int parent_tree_id = (*in_out_cache)->tree.data().parent_tree_id;
  if (parent_tree_id < 0)
    return nullptr;

  TreeCache* parent_cache = GetTreeCacheFromTreeID(parent_tree_id);
  if (!parent_cache)
    return nullptr;

  if (parent_cache->parent_node_id_from_parent_tree > 0) {
    ui::AXNode* parent = parent_cache->tree.GetFromId(
        parent_cache->parent_node_id_from_parent_tree);
    if (parent) {
      int parent_child_tree_id =
          parent->data().GetIntAttribute(ui::AX_ATTR_CHILD_TREE_ID);
      if (parent_child_tree_id == (*in_out_cache)->tree_id) {
        *in_out_cache = parent_cache;
        return parent;
      }
    }
  }

  ui::AXNode* parent = FindNodeWithChildTreeId(parent_cache->tree.root(),
                                               (*in_out_cache)->tree_id);
  if (parent) {
    (*in_out_cache)->parent_node_id_from_parent_tree = parent->id();
    *in_out_cache = parent_cache;
    return parent;
  }

  return nullptr;
}
