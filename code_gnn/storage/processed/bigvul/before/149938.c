void LayerTreeHostImpl::DistributeScrollDelta(ScrollState* scroll_state) {
  std::list<ScrollNode*> current_scroll_chain;
  ScrollTree& scroll_tree = active_tree_->property_trees()->scroll_tree;
  ScrollNode* scroll_node = scroll_tree.CurrentlyScrollingNode();
  ScrollNode* viewport_scroll_node = OuterViewportScrollNode();
  if (scroll_node) {
    for (; scroll_tree.parent(scroll_node);
         scroll_node = scroll_tree.parent(scroll_node)) {
      if (scroll_node == viewport_scroll_node) {
        DCHECK(viewport_scroll_node);
        current_scroll_chain.push_front(viewport_scroll_node);
        break;
      }

      if (!scroll_node->scrollable)
        continue;

      current_scroll_chain.push_front(scroll_node);
    }
  }
  scroll_state->set_scroll_chain_and_layer_tree(current_scroll_chain,
                                                active_tree());
  scroll_state->DistributeToScrollChainDescendant();
}
