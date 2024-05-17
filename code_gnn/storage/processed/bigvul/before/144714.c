WebContentsImpl::WebContentsTreeNode::~WebContentsTreeNode() {
  if (outer_web_contents_) {
    ChildrenSet& child_ptrs_in_parent =
        outer_web_contents_->node_->inner_web_contents_tree_nodes_;
    ChildrenSet::iterator iter = child_ptrs_in_parent.find(this);
    DCHECK(iter != child_ptrs_in_parent.end());
    child_ptrs_in_parent.erase(this);
  }

  for (WebContentsTreeNode* child : inner_web_contents_tree_nodes_)
    child->outer_web_contents_ = nullptr;
}
