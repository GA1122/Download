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
    render_frame->frame_->SetCommittedFirstRealLoad();

  RenderWidget* render_widget = render_view->GetWidget();

  auto* web_frame_widget = blink::WebFrameWidget::CreateForMainFrame(
      render_view->WidgetClient(), web_frame);
  render_view->AttachWebFrameWidget(web_frame_widget);
  render_widget->UpdateWebViewWithDeviceScaleFactor();

  render_frame->render_widget_ = render_widget;
  render_frame->in_frame_tree_ = true;
  render_frame->Initialize();

  return render_frame;
}
