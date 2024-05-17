v8::MaybeLocal<v8::Object> ModuleSystem::RequireNativeFromString(
    const std::string& native_name) {
  if (natives_enabled_ == 0) {
    if (exception_handler_) {
      GetIsolate()->ThrowException(
          ToV8StringUnsafe(GetIsolate(), "Natives disabled"));
      return v8::MaybeLocal<v8::Object>();
    }
    Fatal(context_, "Natives disabled for requireNative(" + native_name + ")");
    return v8::MaybeLocal<v8::Object>();
  }

  if (overridden_native_handlers_.count(native_name) > 0u) {
    v8::Local<v8::Value> value = RequireForJsInner(
        ToV8StringUnsafe(GetIsolate(), native_name.c_str()));
    if (value.IsEmpty() || !value->IsObject())
      return v8::MaybeLocal<v8::Object>();
    return value.As<v8::Object>();
  }

  NativeHandlerMap::iterator i = native_handler_map_.find(native_name);
  if (i == native_handler_map_.end()) {
    Fatal(context_,
          "Couldn't find native for requireNative(" + native_name + ")");
    return v8::MaybeLocal<v8::Object>();
  }
  return i->second->NewInstance();
}
