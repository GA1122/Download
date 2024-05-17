void RenderFrameImpl::JavaScriptIsolatedWorldRequest::Completed(
    const blink::WebVector<v8::Local<v8::Value>>& result) {
  if (!render_frame_impl_.get()) {
    return;
  }

  if (notify_result_) {
    base::ListValue list;
    if (!result.IsEmpty()) {
      v8::Local<v8::Context> context =
          render_frame_impl_.get()->frame_->MainWorldScriptContext();
      v8::Context::Scope context_scope(context);
      V8ValueConverterImpl converter;
      converter.SetDateAllowed(true);
      converter.SetRegExpAllowed(true);
      for (const auto& value : result) {
        std::unique_ptr<base::Value> result_value(
            converter.FromV8Value(value, context));
        list.Append(result_value ? std::move(result_value)
                                 : std::make_unique<base::Value>());
      }
    } else {
      list.Set(0, std::make_unique<base::Value>());
    }
    render_frame_impl_.get()->Send(
        new FrameHostMsg_JavaScriptExecuteResponse(routing_id_, id_, list));
  }

  delete this;
}
