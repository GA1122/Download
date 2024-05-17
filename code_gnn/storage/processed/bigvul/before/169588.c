void CastStreamingNativeHandler::SetDestinationCastUdpTransport(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
  CHECK_EQ(2, args.Length());
  CHECK(args[0]->IsInt32());
  CHECK(args[1]->IsObject());

  const int transport_id = args[0]->ToInt32(args.GetIsolate())->Value();
  CastUdpTransport* transport = GetUdpTransportOrThrow(transport_id);
  if (!transport)
    return;

  net::IPEndPoint dest;
  if (!IPEndPointFromArg(args.GetIsolate(),
                         args[1],
                         &dest)) {
    return;
  }
  transport->SetDestination(
      dest,
      base::Bind(&CastStreamingNativeHandler::CallErrorCallback,
                 weak_factory_.GetWeakPtr(),
                 transport_id));
}
