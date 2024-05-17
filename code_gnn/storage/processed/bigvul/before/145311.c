void RuntimeCustomBindings::GetManifest(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
  CHECK(context()->extension());

  scoped_ptr<V8ValueConverter> converter(V8ValueConverter::create());
  args.GetReturnValue().Set(converter->ToV8Value(
      context()->extension()->manifest()->value(), context()->v8_context()));
}
