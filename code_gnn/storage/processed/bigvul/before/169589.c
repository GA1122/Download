void CastStreamingNativeHandler::SetOptionsCastUdpTransport(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
  CHECK_EQ(2, args.Length());
  CHECK(args[0]->IsInt32());
  CHECK(args[1]->IsObject());

  const int transport_id = args[0]->ToInt32(args.GetIsolate())->Value();
  CastUdpTransport* transport = GetUdpTransportOrThrow(transport_id);
  if (!transport)
    return;

  scoped_ptr<V8ValueConverter> converter(V8ValueConverter::create());
  base::Value* options_value =
      converter->FromV8Value(args[1], context()->v8_context());
  base::DictionaryValue* options;
  if (!options_value || !options_value->GetAsDictionary(&options)) {
    delete options_value;
    args.GetIsolate()->ThrowException(v8::Exception::TypeError(
        v8::String::NewFromUtf8(args.GetIsolate(), kUnableToConvertArgs)));
    return;
  }
  transport->SetOptions(make_scoped_ptr(options));
}
