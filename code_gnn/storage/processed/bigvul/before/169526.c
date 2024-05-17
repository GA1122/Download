void AutomationInternalCustomBindings::Invalidate() {
  ObjectBackedNativeHandler::Invalidate();

  if (message_filter_)
    message_filter_->Detach();

  for (auto iter : tree_id_to_tree_cache_map_) {
    TreeCache* cache = iter.second;
    cache->tree.SetDelegate(nullptr);
    delete cache;
  }
  tree_id_to_tree_cache_map_.clear();
}
