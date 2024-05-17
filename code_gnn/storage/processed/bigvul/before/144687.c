void WebContentsImpl::Stop() {
  for (FrameTreeNode* node : frame_tree_.Nodes())
    node->StopLoading();
  FOR_EACH_OBSERVER(WebContentsObserver, observers_, NavigationStopped());
}
