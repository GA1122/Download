RenderWidgetHostImpl* WebContentsImpl::GetFocusedRenderWidgetHost(
    RenderWidgetHostImpl* receiving_widget) {
  if (!SiteIsolationPolicy::AreCrossProcessFramesPossible())
    return receiving_widget;

  if (receiving_widget != GetMainFrame()->GetRenderWidgetHost())
    return receiving_widget;

  FrameTreeNode* focused_frame = frame_tree_.GetFocusedFrame();
  if (!focused_frame)
    return receiving_widget;

  RenderWidgetHostView* view = focused_frame->current_frame_host()->GetView();
  if (!view)
    return nullptr;

  return RenderWidgetHostImpl::From(view->GetRenderWidgetHost());
}
