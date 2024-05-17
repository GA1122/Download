void ModuleSystem::Private(const v8::FunctionCallbackInfo<v8::Value>& args) {
  CHECK_EQ(1, args.Length());
  if (!args[0]->IsObject() || args[0]->IsNull()) {
    GetIsolate()->ThrowException(
        v8::Exception::TypeError(ToV8StringUnsafe(GetIsolate(),
            args[0]->IsUndefined()
                ? "Method called without a valid receiver (this). "
                  "Did you forget to call .bind()?"
                : "Invalid invocation: receiver is not an object!")));
    return;
  }
  v8::Local<v8::Object> obj = args[0].As<v8::Object>();
  v8::Local<v8::Value> privates;
  if (!GetPrivate(obj, "privates", &privates) || !privates->IsObject()) {
    privates = v8::Object::New(args.GetIsolate());
    if (privates.IsEmpty()) {
      GetIsolate()->ThrowException(
          ToV8StringUnsafe(GetIsolate(), "Failed to create privates"));
      return;
    }
    SetPrivate(obj, "privates", privates);
  }
  args.GetReturnValue().Set(privates);
}
