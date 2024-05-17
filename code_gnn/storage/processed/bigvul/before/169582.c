void CastStreamingNativeHandler::GetStats(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
  CHECK_EQ(2, args.Length());
  CHECK(args[0]->IsInt32());
  CHECK(args[1]->IsFunction());
  const int transport_id = args[0]->ToInt32(args.GetIsolate())->Value();
  CastRtpStream* transport = GetRtpStreamOrThrow(transport_id);
  if (!transport)
    return;

  linked_ptr<v8::Global<v8::Function>> callback(new v8::Global<v8::Function>(
      args.GetIsolate(), args[1].As<v8::Function>()));
  get_stats_callbacks_.insert(std::make_pair(transport_id, callback));

  transport->GetStats(
      base::Bind(&CastStreamingNativeHandler::CallGetStatsCallback,
                 weak_factory_.GetWeakPtr(),
                 transport_id));
}
