blink::WebLocalFrame* RenderFrameImpl::CreateChildFrame(
    blink::WebLocalFrame* parent,
    blink::WebTreeScopeType scope,
    const blink::WebString& name,
    const blink::WebString& fallback_name,
    blink::WebSandboxFlags sandbox_flags,
    const blink::ParsedFeaturePolicy& container_policy,
    const blink::WebFrameOwnerProperties& frame_owner_properties) {
  DCHECK_EQ(frame_, parent);

  int child_routing_id = MSG_ROUTING_NONE;
  mojo::MessagePipeHandle child_interface_provider_handle;
  base::UnguessableToken devtools_frame_token;
  FrameHostMsg_CreateChildFrame_Params params;
  params.parent_routing_id = routing_id_;
  params.scope = scope;
  params.frame_name = name.Utf8();
  params.is_created_by_script =
      v8::Isolate::GetCurrent() && v8::Isolate::GetCurrent()->InContext();
  params.frame_unique_name = unique_name_helper_.GenerateNameForNewChildFrame(
      params.frame_name.empty() ? fallback_name.Utf8() : params.frame_name,
      params.is_created_by_script);
  params.frame_policy = {sandbox_flags, container_policy};
  params.frame_owner_properties =
      ConvertWebFrameOwnerPropertiesToFrameOwnerProperties(
          frame_owner_properties);
  Send(new FrameHostMsg_CreateChildFrame(params, &child_routing_id,
                                         &child_interface_provider_handle,
                                         &devtools_frame_token));

  if (child_routing_id == MSG_ROUTING_NONE)
    return nullptr;

  CHECK(child_interface_provider_handle.is_valid());
  service_manager::mojom::InterfaceProviderPtr child_interface_provider;
  child_interface_provider.Bind(
      service_manager::mojom::InterfaceProviderPtrInfo(
          mojo::ScopedMessagePipeHandle(child_interface_provider_handle), 0u));


  TRACE_EVENT2("navigation,rail", "RenderFrameImpl::createChildFrame",
               "id", routing_id_,
               "child", child_routing_id);

  RenderFrameImpl* child_render_frame = RenderFrameImpl::Create(
      render_view_, child_routing_id, std::move(child_interface_provider),
      devtools_frame_token);
  child_render_frame->unique_name_helper_.set_propagated_name(
      params.frame_unique_name);
  if (params.is_created_by_script)
    child_render_frame->unique_name_helper_.Freeze();
  child_render_frame->InitializeBlameContext(this);
  blink::WebLocalFrame* web_frame = parent->CreateLocalChild(
      scope, child_render_frame,
      child_render_frame->blink_interface_registry_.get());

  child_render_frame->in_frame_tree_ = true;
  child_render_frame->Initialize();

  return web_frame;
}
