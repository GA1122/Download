LayerTreeHostImpl::BuildEvictionQueue(TreePriority tree_priority) {
  TRACE_EVENT0(TRACE_DISABLED_BY_DEFAULT("cc.debug"),
               "LayerTreeHostImpl::BuildEvictionQueue");

  std::unique_ptr<EvictionTilePriorityQueue> queue(
      new EvictionTilePriorityQueue);
  queue->Build(active_tree_->picture_layers(),
               pending_tree_ ? pending_tree_->picture_layers()
                             : std::vector<PictureLayerImpl*>(),
               tree_priority);
  return queue;
}
