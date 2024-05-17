v8::MaybeLocal<v8::Object> ModuleSystem::Require(
    const std::string& module_name) {
  v8::Local<v8::String> v8_module_name;
  if (!ToV8String(GetIsolate(), module_name, &v8_module_name))
    return v8::MaybeLocal<v8::Object>();
  v8::EscapableHandleScope handle_scope(GetIsolate());
  v8::Local<v8::Value> value = RequireForJsInner(
      v8_module_name);
  if (value.IsEmpty() || !value->IsObject())
    return v8::MaybeLocal<v8::Object>();
  return handle_scope.Escape(value.As<v8::Object>());
}
