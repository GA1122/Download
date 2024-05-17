v8::Local<v8::Value> ModuleSystem::GetSource(const std::string& module_name) {
  v8::EscapableHandleScope handle_scope(GetIsolate());
  if (!source_map_->Contains(module_name))
    return v8::Undefined(GetIsolate());
  return handle_scope.Escape(
      v8::Local<v8::Value>(source_map_->GetSource(GetIsolate(), module_name)));
}
