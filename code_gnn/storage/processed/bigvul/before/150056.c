void LayerTreeHostImpl::SetTreeLayerOpacityMutated(ElementId element_id,
                                                   LayerTreeImpl* tree,
                                                   float opacity) {
  if (!tree)
    return;

  PropertyTrees* property_trees = tree->property_trees();
  DCHECK_EQ(1u,
            property_trees->element_id_to_effect_node_index.count(element_id));
  const int effect_node_index =
      property_trees->element_id_to_effect_node_index[element_id];
  property_trees->effect_tree.OnOpacityAnimated(opacity, effect_node_index,
                                                tree);
}
