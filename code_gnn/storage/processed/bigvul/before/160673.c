void RenderFrameImpl::OnPostMessageEvent(
    const FrameMsg_PostMessage_Params& params) {
  WebFrame* source_frame = nullptr;
  if (params.source_routing_id != MSG_ROUTING_NONE) {
    RenderFrameProxy* source_proxy =
        RenderFrameProxy::FromRoutingID(params.source_routing_id);
    if (source_proxy)
      source_frame = source_proxy->web_frame();
  }

  WebSerializedScriptValue serialized_script_value;
  if (params.is_data_raw_string) {
    v8::Isolate* isolate = blink::MainThreadIsolate();
    v8::HandleScope handle_scope(isolate);
    v8::Local<v8::Context> context = frame_->MainWorldScriptContext();
    v8::Context::Scope context_scope(context);
    V8ValueConverterImpl converter;
    converter.SetDateAllowed(true);
    converter.SetRegExpAllowed(true);
    std::unique_ptr<base::Value> value(new base::Value(params.data));
    v8::Local<v8::Value> result_value = converter.ToV8Value(value.get(),
                                                             context);
    serialized_script_value =
        WebSerializedScriptValue::Serialize(isolate, result_value);
  } else {
    serialized_script_value =
        WebSerializedScriptValue::FromString(WebString::FromUTF16(params.data));
  }

  WebSecurityOrigin target_origin;
  if (!params.target_origin.empty()) {
    target_origin = WebSecurityOrigin::CreateFromString(
        WebString::FromUTF16(params.target_origin));
  }

  WebDOMMessageEvent msg_event(
      serialized_script_value, WebString::FromUTF16(params.source_origin),
      source_frame, frame_->GetDocument(), std::move(params.message_ports));
  frame_->DispatchMessageEventWithOriginCheck(target_origin, msg_event);
}
