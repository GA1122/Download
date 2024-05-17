void ModuleSystem::OnModuleLoaded(
    scoped_ptr<v8::Global<v8::Promise::Resolver>> resolver,
    v8::Local<v8::Value> value) {
  if (!is_valid())
    return;
  v8::HandleScope handle_scope(GetIsolate());
  v8::Local<v8::Promise::Resolver> resolver_local(
      v8::Local<v8::Promise::Resolver>::New(GetIsolate(), *resolver));
  resolver_local->Resolve(context()->v8_context(), value);
}
