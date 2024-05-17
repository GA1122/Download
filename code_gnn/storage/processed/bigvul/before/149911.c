static void CollectScrollbarUpdates(
    ScrollAndScaleSet* scroll_info,
    std::unordered_map<int, std::unique_ptr<ScrollbarAnimationController>>*
        controllers) {
  scroll_info->scrollbars.reserve(controllers->size());
  for (auto& pair : *controllers) {
    scroll_info->scrollbars.push_back(LayerTreeHostCommon::ScrollbarsUpdateInfo(
        pair.first, pair.second->ScrollbarsHidden()));
  }
}
