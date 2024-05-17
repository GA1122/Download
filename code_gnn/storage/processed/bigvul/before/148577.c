void WebContentsImpl::Stop() {
  for (FrameTreeNode* node : frame_tree_.Nodes())
    node->StopLoading();
  for (auto& observer : observers_)
    observer.NavigationStopped();
}
