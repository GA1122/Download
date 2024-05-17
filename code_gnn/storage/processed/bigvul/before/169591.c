void CastStreamingNativeHandler::StartCastRtpStream(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
  CHECK_EQ(2, args.Length());
  CHECK(args[0]->IsInt32());
  CHECK(args[1]->IsObject());

  const int transport_id = args[0]->ToInt32(args.GetIsolate())->Value();
  CastRtpStream* transport = GetRtpStreamOrThrow(transport_id);
  if (!transport)
    return;

  scoped_ptr<V8ValueConverter> converter(V8ValueConverter::create());
  scoped_ptr<base::Value> params_value(
      converter->FromV8Value(args[1], context()->v8_context()));
  if (!params_value) {
    args.GetIsolate()->ThrowException(v8::Exception::TypeError(
        v8::String::NewFromUtf8(args.GetIsolate(), kUnableToConvertParams)));
    return;
  }
  scoped_ptr<RtpParams> params = RtpParams::FromValue(*params_value);
  if (!params) {
    args.GetIsolate()->ThrowException(v8::Exception::TypeError(
        v8::String::NewFromUtf8(args.GetIsolate(), kInvalidRtpParams)));
    return;
  }

  CastRtpParams cast_params;
  v8::Isolate* isolate = context()->v8_context()->GetIsolate();
  if (!ToCastRtpParamsOrThrow(isolate, *params, &cast_params))
    return;

  base::Closure start_callback =
      base::Bind(&CastStreamingNativeHandler::CallStartCallback,
                 weak_factory_.GetWeakPtr(),
                 transport_id);
  base::Closure stop_callback =
      base::Bind(&CastStreamingNativeHandler::CallStopCallback,
                 weak_factory_.GetWeakPtr(),
                 transport_id);
  CastRtpStream::ErrorCallback error_callback =
      base::Bind(&CastStreamingNativeHandler::CallErrorCallback,
                 weak_factory_.GetWeakPtr(),
                 transport_id);
  transport->Start(cast_params, start_callback, stop_callback, error_callback);
}
