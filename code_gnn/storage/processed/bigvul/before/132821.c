scoped_ptr<TilingSetEvictionQueue> PictureLayerImpl::CreateEvictionQueue(
    TreePriority tree_priority) {
  if (!tilings_)
    return make_scoped_ptr(new TilingSetEvictionQueue());
  bool skip_shared_out_of_order_tiles =
      GetPendingOrActiveTwinLayer() != nullptr;
  return make_scoped_ptr(new TilingSetEvictionQueue(
      tilings_.get(), tree_priority, skip_shared_out_of_order_tiles));
}
