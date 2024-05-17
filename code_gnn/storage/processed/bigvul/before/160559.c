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
  blink::WebLocalFrame* web_frame;
  RenderFrameImpl* render_frame;
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
        ResolveOpener(opener_routing_id));

    render_frame->in_frame_tree_ = true;
  } else {
    RenderFrameProxy* proxy =
        RenderFrameProxy::FromRoutingID(proxy_routing_id);
    if (!proxy)
      return;

    render_frame = RenderFrameImpl::Create(proxy->render_view(), routing_id,
                                           std::move(interface_provider),
                                           devtools_frame_token);
    render_frame->InitializeBlameContext(nullptr);
    render_frame->proxy_routing_id_ = proxy_routing_id;
    proxy->set_provisional_frame_routing_id(routing_id);
    web_frame = blink::WebLocalFrame::CreateProvisional(
        render_frame, render_frame->blink_interface_registry_.get(),
        proxy->web_frame(), replicated_state.frame_policy.sandbox_flags,
        replicated_state.frame_policy.container_policy);
  }
  CHECK(parent_routing_id != MSG_ROUTING_NONE || !web_frame->Parent());

  if (widget_params.routing_id != MSG_ROUTING_NONE) {
    render_frame->render_widget_ = RenderWidget::CreateForFrame(
        widget_params.routing_id, widget_params.hidden,
        render_frame->render_view_->screen_info(), compositor_deps, web_frame);
  }

  if (has_committed_real_load)
    web_frame->SetCommittedFirstRealLoad();

  render_frame->Initialize();
}
