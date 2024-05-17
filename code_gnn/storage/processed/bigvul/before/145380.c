  void GetSchema(const v8::FunctionCallbackInfo<v8::Value>& args) {
    args.GetReturnValue().Set(
        registry_->GetSchema(*v8::String::Utf8Value(args[0])));
  }
