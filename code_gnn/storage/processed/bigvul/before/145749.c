void ModuleSystem::RequireNative(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
  CHECK_EQ(1, args.Length());
  std::string native_name = *v8::String::Utf8Value(args[0]);
  v8::Local<v8::Object> object;
  if (RequireNativeFromString(native_name).ToLocal(&object))
    args.GetReturnValue().Set(object);
}
