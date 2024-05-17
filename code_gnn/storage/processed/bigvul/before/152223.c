void RenderFrameImpl::JavaScriptIsolatedWorldRequest::Completed(
    const blink::WebVector<v8::Local<v8::Value>>& result) {
  if (!render_frame_impl_) {
    delete this;
    return;
  }

  base::Value value;
  if (!result.empty()) {
    v8::Local<v8::Context> context =
        render_frame_impl_.get()->frame_->MainWorldScriptContext();
    v8::Context::Scope context_scope(context);
    V8ValueConverterImpl converter;
    converter.SetDateAllowed(true);
    converter.SetRegExpAllowed(true);
    std::unique_ptr<base::Value> new_value =
        converter.FromV8Value(*result.begin(), context);
    if (new_value)
      value = base::Value::FromUniquePtrValue(std::move(new_value));
  }

  std::move(callback_).Run(std::move(value));

  delete this;
}
