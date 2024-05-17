void RenderFrameImpl::CreateFrame(
    int routing_id,
    service_manager::mojom::InterfaceProviderPtr interface_provider,
    int proxy_routing_id,
    int opener_routing_id,
    int parent_routing_id,
    int previous_sibling_routing_id,
    const base::UnguessableToken& devtools_frame_token,
    const FrameReplicationState& replicated_state,
    CompositorDependencies* compositor_deps,
    const mojom::CreateFrameWidgetParams& widget_params,
    const FrameOwnerProperties& frame_owner_properties,
    bool has_committed_real_load) {
  RenderViewImpl* render_view = nullptr;
  RenderFrameImpl* render_frame = nullptr;
  blink::WebLocalFrame* web_frame = nullptr;
  if (proxy_routing_id == MSG_ROUTING_NONE) {

    RenderFrameProxy* parent_proxy =
        RenderFrameProxy::FromRoutingID(parent_routing_id);
    CHECK(parent_proxy);
    blink::WebRemoteFrame* parent_web_frame = parent_proxy->web_frame();

    blink::WebFrame* previous_sibling_web_frame = nullptr;
    RenderFrameProxy* previous_sibling_proxy =
        RenderFrameProxy::FromRoutingID(previous_sibling_routing_id);
    if (previous_sibling_proxy)
      previous_sibling_web_frame = previous_sibling_proxy->web_frame();

    render_view = parent_proxy->render_view();
    render_frame = RenderFrameImpl::Create(
        parent_proxy->render_view(), routing_id, std::move(interface_provider),
        devtools_frame_token);
    render_frame->InitializeBlameContext(FromRoutingID(parent_routing_id));
    render_frame->unique_name_helper_.set_propagated_name(
        replicated_state.unique_name);
    web_frame = parent_web_frame->CreateLocalChild(
        replicated_state.scope, WebString::FromUTF8(replicated_state.name),
        replicated_state.frame_policy.sandbox_flags, render_frame,
        render_frame->blink_interface_registry_.get(),
        previous_sibling_web_frame,
        replicated_state.frame_policy.container_policy,
        ConvertFrameOwnerPropertiesToWebFrameOwnerProperties(
            frame_owner_properties),
        replicated_state.frame_owner_element_type,
        ResolveOpener(opener_routing_id));

    render_frame->in_frame_tree_ = true;
  } else {
    RenderFrameProxy* proxy =
        RenderFrameProxy::FromRoutingID(proxy_routing_id);
    if (!proxy)
      return;

    const bool proxy_is_main_frame = !proxy->web_frame()->Parent();

    render_view = proxy->render_view();
    render_frame = RenderFrameImpl::Create(render_view, routing_id,
                                           std::move(interface_provider),
                                           devtools_frame_token);
    render_frame->InitializeBlameContext(nullptr);
    render_frame->proxy_routing_id_ = proxy_routing_id;
    proxy->set_provisional_frame_routing_id(routing_id);
    web_frame = blink::WebLocalFrame::CreateProvisional(
        render_frame, render_frame->blink_interface_registry_.get(),
        proxy->web_frame(), replicated_state.frame_policy.sandbox_flags,
        replicated_state.frame_policy.container_policy);
    DCHECK_EQ(proxy_is_main_frame, !web_frame->Parent());
  }

  DCHECK(render_view);
  DCHECK(render_frame);
  DCHECK(web_frame);

  const bool is_main_frame = !web_frame->Parent();

  if (!is_main_frame)
    DCHECK_NE(parent_routing_id, MSG_ROUTING_NONE);

  if (is_main_frame) {

    DCHECK_NE(widget_params.routing_id, MSG_ROUTING_NONE);

    RenderWidget* render_widget = render_view->GetWidget();

    auto* web_frame_widget = blink::WebFrameWidget::CreateForMainFrame(
        render_view->WidgetClient(), web_frame);
    render_view->AttachWebFrameWidget(web_frame_widget);
    render_widget->UpdateWebViewWithDeviceScaleFactor();

    render_widget->WarmupCompositor();

    render_frame->render_widget_ = render_widget;
  } else if (widget_params.routing_id != MSG_ROUTING_NONE) {

    const ScreenInfo& screen_info_from_main_frame =
        render_view->GetWidget()->GetWebScreenInfo();

    scoped_refptr<RenderWidget> render_widget;
    if (g_create_render_widget) {
      render_widget = g_create_render_widget(
          widget_params.routing_id, compositor_deps,
          screen_info_from_main_frame, blink::kWebDisplayModeUndefined,
           false, widget_params.hidden,
           false);
    } else {
      render_widget = base::MakeRefCounted<RenderWidget>(
          widget_params.routing_id, compositor_deps,
          screen_info_from_main_frame, blink::kWebDisplayModeUndefined,
           false, widget_params.hidden,
           false);
    }

    auto* web_frame_widget = blink::WebFrameWidget::CreateForChildLocalRoot(
        render_widget.get(), web_frame);

    render_widget->InitForChildLocalRoot(web_frame_widget);
    render_widget->UpdateWebViewWithDeviceScaleFactor();

    if (g_render_widget_initialized)
      g_render_widget_initialized(render_widget.get());

    render_frame->render_widget_ = std::move(render_widget);
  }

  if (has_committed_real_load)
    render_frame->frame_->SetCommittedFirstRealLoad();

  render_frame->Initialize();
}
