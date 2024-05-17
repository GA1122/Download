  void GetChrome(const v8::FunctionCallbackInfo<v8::Value>& args) {
    args.GetReturnValue().Set(GetOrCreateChrome(context()));
  }
