  int RequestNavigationWithParameters(
      FrameTreeNode* node,
      const GURL& url,
      const Referrer& referrer,
      ui::PageTransition transition_type) {
    NavigationController::LoadURLParams load_params(url);
    load_params.frame_tree_node_id = node->frame_tree_node_id();
    load_params.referrer = referrer;
    load_params.transition_type = transition_type;

    controller().LoadURLWithParams(load_params);
    return controller().GetPendingEntry()->GetUniqueID();
  }
