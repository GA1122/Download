v8::Local<v8::Value> ModuleSystem::CallModuleMethod(
    const std::string& module_name,
    const std::string& method_name,
    int argc,
    v8::Local<v8::Value> argv[]) {
  TRACE_EVENT2("v8",
               "v8.callModuleMethod",
               "module_name",
               module_name,
               "method_name",
               method_name);

  v8::EscapableHandleScope handle_scope(GetIsolate());
  v8::Local<v8::Context> v8_context = context()->v8_context();
  v8::Context::Scope context_scope(v8_context);

  v8::Local<v8::String> v8_module_name;
  v8::Local<v8::String> v8_method_name;
  if (!ToV8String(GetIsolate(), module_name.c_str(), &v8_module_name) ||
      !ToV8String(GetIsolate(), method_name.c_str(), &v8_method_name)) {
    return handle_scope.Escape(v8::Undefined(GetIsolate()));
  }

  v8::Local<v8::Value> module;
  {
    NativesEnabledScope natives_enabled(this);
    module = RequireForJsInner(v8_module_name);
  }

  if (module.IsEmpty() || !module->IsObject()) {
    Fatal(context_,
          "Failed to get module " + module_name + " to call " + method_name);
    return handle_scope.Escape(v8::Undefined(GetIsolate()));
  }

  v8::Local<v8::Object> object = v8::Local<v8::Object>::Cast(module);
  v8::Local<v8::Value> value;
  if (!GetProperty(v8_context, object, v8_method_name, &value) ||
      !value->IsFunction()) {
    Fatal(context_, module_name + "." + method_name + " is not a function");
    return handle_scope.Escape(v8::Undefined(GetIsolate()));
  }

  v8::Local<v8::Function> func = v8::Local<v8::Function>::Cast(value);
  v8::Local<v8::Value> result;
  {
    v8::TryCatch try_catch(GetIsolate());
    try_catch.SetCaptureMessage(true);
    result = context_->CallFunction(func, argc, argv);
    if (try_catch.HasCaught()) {
      HandleException(try_catch);
      result = v8::Undefined(GetIsolate());
    }
  }
  return handle_scope.Escape(result);
}
