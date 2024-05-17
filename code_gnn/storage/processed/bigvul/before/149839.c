void LayerTreeHost::SetElementTransformMutated(
    ElementId element_id,
    ElementListType list_type,
    const gfx::Transform& transform) {
  Layer* layer = LayerByElementId(element_id);
  DCHECK(layer);
  layer->OnTransformAnimated(transform);

  if (property_trees_.IsInIdToIndexMap(PropertyTrees::TreeType::TRANSFORM,
                                       layer->id())) {
    DCHECK_EQ(layer->transform_tree_index(),
              property_trees_.layer_id_to_transform_node_index[layer->id()]);
    TransformNode* node =
        property_trees_.transform_tree.Node(layer->transform_tree_index());
    if (node->local == transform)
      return;

    node->local = transform;
    node->needs_local_transform_update = true;
    node->has_potential_animation = true;
    property_trees_.transform_tree.set_needs_update(true);
  }

  SetNeedsUpdateLayers();
}
