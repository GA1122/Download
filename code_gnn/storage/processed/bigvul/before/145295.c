void ObjectBackedNativeHandler::Invalidate() {
  v8::Isolate* isolate = GetIsolate();
  v8::HandleScope handle_scope(isolate);
  v8::Context::Scope context_scope(context_->v8_context());

  for (size_t i = 0; i < router_data_.Size(); i++) {
    v8::Local<v8::Object> data = router_data_.Get(i);
    v8::Local<v8::Value> handler_function_value;
    CHECK(GetPrivate(data, kHandlerFunction, &handler_function_value));
    delete static_cast<HandlerFunction*>(
        handler_function_value.As<v8::External>()->Value());
    DeletePrivate(data, kHandlerFunction);
  }

  router_data_.Clear();
  object_template_.Reset();

  NativeHandler::Invalidate();
}
