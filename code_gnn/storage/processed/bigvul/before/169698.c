v8::Local<v8::Value> V8ValueConverterImpl::ToV8Value(
    const base::Value* value, v8::Local<v8::Context> context) const {
  v8::Context::Scope context_scope(context);
  v8::EscapableHandleScope handle_scope(context->GetIsolate());
  return handle_scope.Escape(
      ToV8ValueImpl(context->GetIsolate(), context->Global(), value));
}
