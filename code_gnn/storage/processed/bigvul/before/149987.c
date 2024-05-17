ScrollNode* LayerTreeHostImpl::OuterViewportScrollNode() const {
  if (!viewport()->MainScrollLayer())
    return nullptr;
  ScrollTree& scroll_tree = active_tree_->property_trees()->scroll_tree;
  return scroll_tree.Node(viewport()->MainScrollLayer()->scroll_tree_index());
}
