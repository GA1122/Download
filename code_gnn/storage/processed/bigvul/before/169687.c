base::Value* V8ValueConverterImpl::FromV8Value(
    v8::Local<v8::Value> val,
    v8::Local<v8::Context> context) const {
  v8::Context::Scope context_scope(context);
  v8::HandleScope handle_scope(context->GetIsolate());
  FromV8ValueState state(avoid_identity_hash_for_testing_);
  return FromV8ValueImpl(&state, val, context->GetIsolate());
}
