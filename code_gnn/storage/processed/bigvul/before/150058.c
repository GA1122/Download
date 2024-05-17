void LayerTreeHostImpl::SetTreeLayerTransformMutated(
    ElementId element_id,
    LayerTreeImpl* tree,
    const gfx::Transform& transform) {
  if (!tree)
    return;

  PropertyTrees* property_trees = tree->property_trees();
  DCHECK_EQ(
      1u, property_trees->element_id_to_transform_node_index.count(element_id));
  const int transform_node_index =
      property_trees->element_id_to_transform_node_index[element_id];
  property_trees->transform_tree.OnTransformAnimated(
      transform, transform_node_index, tree);
  if (LayerImpl* layer = tree->LayerByElementId(element_id))
    layer->set_was_ever_ready_since_last_transform_animation(false);
}
