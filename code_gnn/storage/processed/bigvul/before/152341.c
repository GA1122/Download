base::Value RenderFrameImpl::GetJavaScriptExecutionResult(
    v8::Local<v8::Value> result) {
  if (!result.IsEmpty()) {
    v8::Local<v8::Context> context = frame_->MainWorldScriptContext();
    v8::Context::Scope context_scope(context);
    V8ValueConverterImpl converter;
    converter.SetDateAllowed(true);
    converter.SetRegExpAllowed(true);
    std::unique_ptr<base::Value> new_value =
        converter.FromV8Value(result, context);
    if (new_value)
      return std::move(*new_value);
  }
  return base::Value();
}
