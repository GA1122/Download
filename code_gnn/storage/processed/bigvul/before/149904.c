std::unique_ptr<RasterTilePriorityQueue> LayerTreeHostImpl::BuildRasterQueue(
    TreePriority tree_priority,
    RasterTilePriorityQueue::Type type) {
  TRACE_EVENT0(TRACE_DISABLED_BY_DEFAULT("cc.debug"),
               "LayerTreeHostImpl::BuildRasterQueue");

  return RasterTilePriorityQueue::Create(active_tree_->picture_layers(),
                                         pending_tree_
                                             ? pending_tree_->picture_layers()
                                             : std::vector<PictureLayerImpl*>(),
                                         tree_priority, type);
}
