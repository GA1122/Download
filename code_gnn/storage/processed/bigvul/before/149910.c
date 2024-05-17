static void CollectScrollDeltas(ScrollAndScaleSet* scroll_info,
                                LayerTreeImpl* tree_impl) {
  if (tree_impl->LayerListIsEmpty())
    return;

  int inner_viewport_layer_id =
      tree_impl->InnerViewportScrollLayer()
          ? tree_impl->InnerViewportScrollLayer()->id()
          : Layer::INVALID_ID;

  tree_impl->property_trees()->scroll_tree.CollectScrollDeltas(
      scroll_info, inner_viewport_layer_id);
}
