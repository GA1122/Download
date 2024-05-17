void CastStreamingNativeHandler::GetRawEvents(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
  CHECK_EQ(3, args.Length());
  CHECK(args[0]->IsInt32());
  CHECK(args[1]->IsNull() || args[1]->IsString());
  CHECK(args[2]->IsFunction());

  const int transport_id = args[0]->ToInt32(args.GetIsolate())->Value();
  linked_ptr<v8::Global<v8::Function>> callback(new v8::Global<v8::Function>(
      args.GetIsolate(), args[2].As<v8::Function>()));
  std::string extra_data;
  if (!args[1]->IsNull()) {
    extra_data = *v8::String::Utf8Value(args[1]);
  }

  CastRtpStream* transport = GetRtpStreamOrThrow(transport_id);
  if (!transport)
    return;

  get_raw_events_callbacks_.insert(std::make_pair(transport_id, callback));

  transport->GetRawEvents(
      base::Bind(&CastStreamingNativeHandler::CallGetRawEventsCallback,
                 weak_factory_.GetWeakPtr(),
                 transport_id),
      extra_data);
}
