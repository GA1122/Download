v8::Local<v8::Context> V8SchemaRegistry::GetOrCreateContext(
    v8::Isolate* isolate) {
  if (!context_holder_) {
    context_holder_.reset(new gin::ContextHolder(isolate));
    context_holder_->SetContext(v8::Context::New(isolate));
    schema_cache_.reset(new SchemaCache(isolate));
    return context_holder_->context();
  }
  return context_holder_->context();
}
