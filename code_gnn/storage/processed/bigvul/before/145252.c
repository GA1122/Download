void Dispatcher::RegisterBinding(const std::string& api_name,
                                 ScriptContext* context) {
  std::string bind_name;
  v8::Local<v8::Object> bind_object =
      GetOrCreateBindObjectIfAvailable(api_name, &bind_name, context);

  if (bind_object.IsEmpty())
    return;

  v8::Local<v8::String> v8_bind_name =
      v8::String::NewFromUtf8(context->isolate(), bind_name.c_str());
  if (bind_object->HasRealNamedProperty(v8_bind_name)) {
    if (bind_object->HasRealNamedCallbackProperty(v8_bind_name))
      return;   
    if (bind_object->Get(v8_bind_name)->IsObject())
      return;   
  }

  ModuleSystem* module_system = context->module_system();
  if (!source_map_.Contains(api_name)) {
    module_system->RegisterNativeHandler(
        api_name,
        scoped_ptr<NativeHandler>(new BindingGeneratingNativeHandler(
            context, api_name, "binding")));
    module_system->SetNativeLazyField(
        bind_object, bind_name, api_name, "binding");
  } else {
    module_system->SetLazyField(bind_object, bind_name, api_name, "binding");
  }
}
