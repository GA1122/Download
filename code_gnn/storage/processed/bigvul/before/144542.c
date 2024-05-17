WebContentsImpl::GetRenderWidgetHostViewsInTree() {
  std::set<RenderWidgetHostView*> set;
  if (ShowingInterstitialPage()) {
    set.insert(GetRenderWidgetHostView());
  } else {
    for (RenderFrameHost* rfh : GetAllFrames()) {
      RenderWidgetHostView* rwhv = static_cast<RenderFrameHostImpl*>(rfh)
                                       ->frame_tree_node()
                                       ->render_manager()
                                       ->GetRenderWidgetHostView();
      set.insert(rwhv);
    }
  }
  return set;
}
