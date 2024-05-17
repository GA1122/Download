v8::Local<v8::Value> ModuleSystem::CallModuleMethod(
    const std::string& module_name,
    const std::string& method_name) {
  v8::EscapableHandleScope handle_scope(GetIsolate());
  v8::Local<v8::Value> no_args;
  return handle_scope.Escape(
      CallModuleMethod(module_name, method_name, 0, &no_args));
}
