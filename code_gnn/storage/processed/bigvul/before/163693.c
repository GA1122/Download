void WebContentsImpl::AttachToOuterWebContentsFrame(
    WebContents* outer_web_contents,
    RenderFrameHost* outer_contents_frame) {
  CHECK(GuestMode::IsCrossProcessFrameGuest(this));
  RenderFrameHostManager* render_manager = GetRenderManager();

  render_manager->InitRenderView(GetRenderViewHost(), nullptr);
  GetMainFrame()->Init();
  if (!render_manager->GetRenderWidgetHostView())
    CreateRenderWidgetHostViewForRenderManager(GetRenderViewHost());

  auto* outer_web_contents_impl =
      static_cast<WebContentsImpl*>(outer_web_contents);
  auto* outer_contents_frame_impl =
      static_cast<RenderFrameHostImpl*>(outer_contents_frame);
  node_.ConnectToOuterWebContents(outer_web_contents_impl,
                                  outer_contents_frame_impl);

  DCHECK(outer_contents_frame);

  render_manager->CreateOuterDelegateProxy(
      outer_contents_frame->GetSiteInstance(), outer_contents_frame_impl);

  render_manager->SetRWHViewForInnerContents(
      render_manager->GetRenderWidgetHostView());

  static_cast<RenderWidgetHostViewChildFrame*>(
      render_manager->GetRenderWidgetHostView())
      ->RegisterFrameSinkId();

  if (outer_web_contents_impl->frame_tree_.GetFocusedFrame() ==
      outer_contents_frame_impl->frame_tree_node()) {
    SetFocusedFrame(frame_tree_.root(), nullptr);
  }

  auto* parent_frame = outer_contents_frame->GetParent();
  GetMainFrame()->set_browser_plugin_embedder_ax_tree_id(
      parent_frame->GetAXTreeID());
  GetMainFrame()->UpdateAXTreeData();

  text_input_manager_.reset(nullptr);
}
