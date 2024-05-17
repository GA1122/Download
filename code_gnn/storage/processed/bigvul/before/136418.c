  int ElementIdToScrollNodeIndex(CompositorElementId element_id) {
    return web_layer_tree_view_->GetLayerTreeHost()
        ->property_trees()
        ->element_id_to_scroll_node_index[element_id];
  }
