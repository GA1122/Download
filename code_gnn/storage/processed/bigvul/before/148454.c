void WebContentsImpl::NotifyPreferencesChanged() {
  std::set<RenderViewHost*> render_view_host_set;
  for (FrameTreeNode* node : frame_tree_.Nodes()) {
    RenderWidgetHost* render_widget_host =
        node->current_frame_host()->GetRenderWidgetHost();
    if (!render_widget_host)
      continue;
    RenderViewHost* render_view_host = RenderViewHost::From(render_widget_host);
    if (!render_view_host)
      continue;
    render_view_host_set.insert(render_view_host);
  }
  for (RenderViewHost* render_view_host : render_view_host_set)
    render_view_host->OnWebkitPreferencesChanged();
}
