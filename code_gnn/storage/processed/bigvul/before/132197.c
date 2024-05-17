void RenderFrameImpl::EnsureMojoBuiltinsAreAvailable(
    v8::Isolate* isolate,
    v8::Local<v8::Context> context) {
  gin::ModuleRegistry* registry = gin::ModuleRegistry::From(context);
  if (registry->available_modules().count(mojo::js::Core::kModuleName))
    return;

  v8::HandleScope handle_scope(isolate);
  registry->AddBuiltinModule(
      isolate, mojo::js::Core::kModuleName, mojo::js::Core::GetModule(isolate));
  registry->AddBuiltinModule(isolate,
                             mojo::js::Support::kModuleName,
                             mojo::js::Support::GetModule(isolate));
  registry->AddBuiltinModule(
      isolate,
      ServiceRegistryJsWrapper::kModuleName,
      ServiceRegistryJsWrapper::Create(isolate, &service_registry_).ToV8());
}
