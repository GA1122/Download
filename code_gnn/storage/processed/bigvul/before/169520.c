bool AutomationInternalCustomBindings::GetFocusInternal(TreeCache* cache,
                                                        TreeCache** out_cache,
                                                        ui::AXNode** out_node) {
  int focus_id = cache->tree.data().focus_id;
  ui::AXNode* focus = cache->tree.GetFromId(focus_id);
  if (!focus)
    return false;

  while (focus->data().HasIntAttribute(ui::AX_ATTR_CHILD_TREE_ID)) {
    int child_tree_id =
        focus->data().GetIntAttribute(ui::AX_ATTR_CHILD_TREE_ID);

    TreeCache* child_cache = GetTreeCacheFromTreeID(child_tree_id);
    if (!child_cache)
      break;

    if (child_cache->tree.data().focused_tree_id > 0) {
      TreeCache* focused_cache =
          GetTreeCacheFromTreeID(child_cache->tree.data().focused_tree_id);
      if (focused_cache)
        child_cache = focused_cache;
    }

    int child_focus_id = child_cache->tree.data().focus_id;
    ui::AXNode* child_focus = child_cache->tree.GetFromId(child_focus_id);
    if (!child_focus)
      break;

    focus = child_focus;
    cache = child_cache;
  }

  *out_cache = cache;
  *out_node = focus;
  return true;
}
