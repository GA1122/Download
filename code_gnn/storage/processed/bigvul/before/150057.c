void LayerTreeHostImpl::SetTreeLayerScrollOffsetMutated(
    ElementId element_id,
    LayerTreeImpl* tree,
    const gfx::ScrollOffset& scroll_offset) {
  if (!tree)
    return;

  const int layer_id = tree->LayerIdByElementId(element_id);
  PropertyTrees* property_trees = tree->property_trees();
  DCHECK_EQ(1u,
            property_trees->element_id_to_scroll_node_index.count(element_id));
  const int scroll_node_index =
      property_trees->element_id_to_scroll_node_index[element_id];
  property_trees->scroll_tree.OnScrollOffsetAnimated(
      layer_id, scroll_node_index, scroll_offset, tree);
  Mutate(CurrentBeginFrameArgs().frame_time);
}
