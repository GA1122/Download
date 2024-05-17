void PropertyTreeManager::CreateCompositorScrollNode(
    const ScrollPaintPropertyNode* scroll_node,
    const cc::TransformNode& scroll_offset_translation) {
  DCHECK(!scroll_node_map_.Contains(scroll_node));

  auto parent_it = scroll_node_map_.find(scroll_node->Parent());
  DCHECK(parent_it != scroll_node_map_.end());
  int parent_id = parent_it->value;
  int id = GetScrollTree().Insert(cc::ScrollNode(), parent_id);

  cc::ScrollNode& compositor_node = *GetScrollTree().Node(id);
  compositor_node.scrollable = true;

  compositor_node.container_bounds =
      static_cast<gfx::Size>(scroll_node->ContainerRect().Size());
  compositor_node.bounds =
      static_cast<gfx::Size>(scroll_node->ContentsRect().Size());
  compositor_node.user_scrollable_horizontal =
      scroll_node->UserScrollableHorizontal();
  compositor_node.user_scrollable_vertical =
      scroll_node->UserScrollableVertical();
  compositor_node.main_thread_scrolling_reasons =
      scroll_node->GetMainThreadScrollingReasons();

  auto compositor_element_id = scroll_node->GetCompositorElementId();
  if (compositor_element_id) {
    compositor_node.element_id = compositor_element_id;
    property_trees_.element_id_to_scroll_node_index[compositor_element_id] = id;
  }

  compositor_node.transform_id = scroll_offset_translation.id;


  auto result = scroll_node_map_.Set(scroll_node, id);
  DCHECK(result.is_new_entry);

  GetScrollTree().SetScrollOffset(compositor_element_id,
                                  scroll_offset_translation.scroll_offset);
  GetScrollTree().set_needs_update(true);
}
