bool LayerTreeHostImpl::IsCurrentlyScrollingViewport() const {
  auto* node = CurrentlyScrollingNode();
  if (!node)
    return false;
  if (!viewport()->MainScrollLayer())
    return false;
  return node->id == viewport()->MainScrollLayer()->scroll_tree_index();
}
