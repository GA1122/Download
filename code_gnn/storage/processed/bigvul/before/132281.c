void RenderFrameImpl::JavaScriptIsolatedWorldRequest::completed(
    const blink::WebVector<v8::Local<v8::Value>>& result) {
  if (!render_frame_impl_.get()) {
    return;
  }

  if (notify_result_) {
    base::ListValue list;
    if (!result.isEmpty()) {
      v8::Local<v8::Context> context =
          render_frame_impl_.get()->frame_->mainWorldScriptContext();
      v8::Context::Scope context_scope(context);
      V8ValueConverterImpl converter;
      converter.SetDateAllowed(true);
      converter.SetRegExpAllowed(true);
      for (const auto& value : result) {
        scoped_ptr<base::Value> result_value(
            converter.FromV8Value(value, context));
        list.Append(result_value ? result_value.Pass()
                                 : base::Value::CreateNullValue());
      }
    } else {
      list.Set(0, base::Value::CreateNullValue());
    }
    render_frame_impl_.get()->Send(
        new FrameHostMsg_JavaScriptExecuteResponse(routing_id_, id_, list));
  }

  delete this;
}
