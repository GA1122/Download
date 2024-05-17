void WebContentsImpl::NotifyPreferencesChanged() {
  std::set<RenderViewHost*> render_view_host_set;
  for (FrameTreeNode* node : frame_tree_.Nodes()) {
    render_view_host_set.insert(
        node->current_frame_host()->GetRenderViewHost());
  }

  for (RenderViewHost* render_view_host : render_view_host_set)
    render_view_host->OnWebkitPreferencesChanged();
}
