void ModuleSystem::RequireAsync(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
  CHECK_EQ(1, args.Length());
  std::string module_name = *v8::String::Utf8Value(args[0]);
  v8::Local<v8::Context> v8_context = context_->v8_context();
  v8::Local<v8::Promise::Resolver> resolver(
      v8::Promise::Resolver::New(v8_context).ToLocalChecked());
  args.GetReturnValue().Set(resolver->GetPromise());
  scoped_ptr<v8::Global<v8::Promise::Resolver>> global_resolver(
      new v8::Global<v8::Promise::Resolver>(GetIsolate(), resolver));
  gin::ModuleRegistry* module_registry =
      gin::ModuleRegistry::From(v8_context);
  if (!module_registry) {
    Warn(GetIsolate(), "Extension view no longer exists");
    resolver->Reject(v8_context, v8::Exception::Error(ToV8StringUnsafe(
        GetIsolate(), "Extension view no longer exists")));
    return;
  }
  module_registry->LoadModule(
      GetIsolate(), module_name,
      base::Bind(&ModuleSystem::OnModuleLoaded, weak_factory_.GetWeakPtr(),
                 base::Passed(&global_resolver)));
  if (module_registry->available_modules().count(module_name) == 0)
    LoadModule(module_name);
}
