blink::WebLocalFrame* RenderFrameImpl::CreateChildFrame(
    blink::WebLocalFrame* parent,
    blink::WebTreeScopeType scope,
    const blink::WebString& name,
    const blink::WebString& fallback_name,
    const blink::FramePolicy& frame_policy,
    const blink::WebFrameOwnerProperties& frame_owner_properties,
    blink::FrameOwnerElementType frame_owner_element_type) {
  DCHECK_EQ(frame_, parent);

  FrameHostMsg_CreateChildFrame_Params params;
  params.parent_routing_id = routing_id_;
  params.scope = scope;
  params.frame_name = name.Utf8();

  FrameHostMsg_CreateChildFrame_Params_Reply params_reply;

  params.is_created_by_script =
      v8::Isolate::GetCurrent() && v8::Isolate::GetCurrent()->InContext();
  params.frame_unique_name = unique_name_helper_.GenerateNameForNewChildFrame(
      params.frame_name.empty() ? fallback_name.Utf8() : params.frame_name,
      params.is_created_by_script);
  params.frame_policy = frame_policy;
  params.frame_owner_properties =
      ConvertWebFrameOwnerPropertiesToFrameOwnerProperties(
          frame_owner_properties);
  params.frame_owner_element_type = frame_owner_element_type;
  if (!Send(new FrameHostMsg_CreateChildFrame(params, &params_reply))) {
    return nullptr;
  }

  DCHECK(params_reply.new_interface_provider.is_valid());
  service_manager::mojom::InterfaceProviderPtr child_interface_provider;
  child_interface_provider.Bind(
      service_manager::mojom::InterfaceProviderPtrInfo(
          mojo::ScopedMessagePipeHandle(params_reply.new_interface_provider),
          0u),
      GetTaskRunner(blink::TaskType::kInternalIPC));

  DCHECK(params_reply.document_interface_broker_content_handle.is_valid());
  DCHECK(params_reply.document_interface_broker_blink_handle.is_valid());

  blink::mojom::DocumentInterfaceBrokerPtr document_interface_broker_content;
  document_interface_broker_content.Bind(
      blink::mojom::DocumentInterfaceBrokerPtrInfo(
          mojo::ScopedMessagePipeHandle(
              params_reply.document_interface_broker_content_handle),
          blink::mojom::DocumentInterfaceBroker::Version_),
      GetTaskRunner(blink::TaskType::kInternalIPC));

  blink::mojom::DocumentInterfaceBrokerPtr document_interface_broker_blink;
  document_interface_broker_blink.Bind(
      blink::mojom::DocumentInterfaceBrokerPtrInfo(
          mojo::ScopedMessagePipeHandle(
              params_reply.document_interface_broker_blink_handle),
          blink::mojom::DocumentInterfaceBroker::Version_),
      GetTaskRunner(blink::TaskType::kInternalIPC));


  TRACE_EVENT2("navigation,rail", "RenderFrameImpl::createChildFrame", "id",
               routing_id_, "child", params_reply.child_routing_id);

  RenderFrameImpl* child_render_frame =
      RenderFrameImpl::Create(render_view_, params_reply.child_routing_id,
                              std::move(child_interface_provider),
                              std::move(document_interface_broker_content),
                              params_reply.devtools_frame_token);
  child_render_frame->unique_name_helper_.set_propagated_name(
      params.frame_unique_name);
  if (params.is_created_by_script)
    child_render_frame->unique_name_helper_.Freeze();
  child_render_frame->InitializeBlameContext(this);
  blink::WebLocalFrame* web_frame = parent->CreateLocalChild(
      scope, child_render_frame,
      child_render_frame->blink_interface_registry_.get(),
      document_interface_broker_blink.PassInterface().PassHandle());

  child_render_frame->in_frame_tree_ = true;
  child_render_frame->Initialize();

  return web_frame;
}
