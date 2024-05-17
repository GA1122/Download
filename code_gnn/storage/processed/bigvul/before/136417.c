  int ElementIdToEffectNodeIndex(CompositorElementId element_id) {
    return web_layer_tree_view_->GetLayerTreeHost()
        ->property_trees()
        ->element_id_to_effect_node_index[element_id];
  }
