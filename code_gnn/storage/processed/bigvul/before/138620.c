  int RequestNavigation(FrameTreeNode* node, const GURL& url) {
    return RequestNavigationWithParameters(node, url, Referrer(),
                                           ui::PAGE_TRANSITION_LINK);
  }
