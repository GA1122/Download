void WebContentsImpl::ReattachToOuterWebContentsFrame() {
  DCHECK(node_.outer_web_contents());
  auto* render_manager = GetRenderManager();
  auto* parent_frame =
      node_.OuterContentsFrameTreeNode()->current_frame_host()->GetParent();
  render_manager->SetRWHViewForInnerContents(
      render_manager->GetRenderWidgetHostView());

  static_cast<RenderWidgetHostViewChildFrame*>(
      render_manager->GetRenderWidgetHostView())
      ->RegisterFrameSinkId();

  GetMainFrame()->set_browser_plugin_embedder_ax_tree_id(
      parent_frame->GetAXTreeID());
  GetMainFrame()->UpdateAXTreeData();
}
