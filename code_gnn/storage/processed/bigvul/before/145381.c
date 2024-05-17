v8::Local<v8::Array> V8SchemaRegistry::GetSchemas(
    const std::vector<std::string>& apis) {
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  v8::EscapableHandleScope handle_scope(isolate);
  v8::Context::Scope context_scope(GetOrCreateContext(isolate));

  v8::Local<v8::Array> v8_apis(v8::Array::New(isolate, apis.size()));
  size_t api_index = 0;
  for (std::vector<std::string>::const_iterator i = apis.begin();
       i != apis.end();
       ++i) {
    v8_apis->Set(api_index++, GetSchema(*i));
  }
  return handle_scope.Escape(v8_apis);
}
