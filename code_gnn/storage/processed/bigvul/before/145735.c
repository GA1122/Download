void ModuleSystem::LazyFieldGetter(
    v8::Local<v8::Name> property,
    const v8::PropertyCallbackInfo<v8::Value>& info) {
  LazyFieldGetterInner(property.As<v8::String>(), info, &ModuleSystem::Require);
}
