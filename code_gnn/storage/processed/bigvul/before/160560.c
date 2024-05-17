RenderFrameImpl* RenderFrameImpl::CreateMainFrame(
    RenderViewImpl* render_view,
    int32_t routing_id,
    service_manager::mojom::InterfaceProviderPtr interface_provider,
    int32_t widget_routing_id,
    bool hidden,
    const ScreenInfo& screen_info,
    CompositorDependencies* compositor_deps,
    blink::WebFrame* opener,
    const base::UnguessableToken& devtools_frame_token,
    const FrameReplicationState& replicated_state,
    bool has_committed_real_load) {
  DCHECK_NE(MSG_ROUTING_NONE, widget_routing_id);

  RenderFrameImpl* render_frame = RenderFrameImpl::Create(
      render_view, routing_id, std::move(interface_provider),
      devtools_frame_token);
  render_frame->InitializeBlameContext(nullptr);
  WebLocalFrame* web_frame = WebLocalFrame::CreateMainFrame(
      render_view->webview(), render_frame,
      render_frame->blink_interface_registry_.get(), opener,
      WebString::FromUTF8(replicated_state.name),
      replicated_state.frame_policy.sandbox_flags);
  if (has_committed_real_load)
    web_frame->SetCommittedFirstRealLoad();
  render_frame->render_widget_ = RenderWidget::CreateForFrame(
      widget_routing_id, hidden, screen_info, compositor_deps, web_frame);
  DCHECK_EQ(render_view->GetWidget(), render_frame->render_widget_)
      << "Main frame is no longer reusing the RenderView as its widget! "
      << "Does the RenderFrame need to register itself with the RenderWidget?";
  render_frame->in_frame_tree_ = true;
  return render_frame;
}
