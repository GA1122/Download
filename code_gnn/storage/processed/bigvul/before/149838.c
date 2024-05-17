void LayerTreeHost::SetElementOpacityMutated(ElementId element_id,
                                             ElementListType list_type,
                                             float opacity) {
  Layer* layer = LayerByElementId(element_id);
  DCHECK(layer);
  DCHECK_GE(opacity, 0.f);
  DCHECK_LE(opacity, 1.f);
  layer->OnOpacityAnimated(opacity);

  if (property_trees_.IsInIdToIndexMap(PropertyTrees::TreeType::EFFECT,
                                       layer->id())) {
    DCHECK_EQ(layer->effect_tree_index(),
              property_trees_.layer_id_to_effect_node_index[layer->id()]);
    EffectNode* node =
        property_trees_.effect_tree.Node(layer->effect_tree_index());
    if (node->opacity == opacity)
      return;

    node->opacity = opacity;
    property_trees_.effect_tree.set_needs_update(true);
  }

  SetNeedsUpdateLayers();
}
