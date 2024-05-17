Response InspectorNetworkAgent::emulateNetworkConditions(
    bool offline,
    double latency,
    double download_throughput,
    double upload_throughput,
    Maybe<String> connection_type) {
  if (!IsMainThread())
    return Response::Error("Not supported");

  WebConnectionType type = kWebConnectionTypeUnknown;
  if (connection_type.isJust()) {
    type = ToWebConnectionType(connection_type.fromJust());
    if (type == kWebConnectionTypeUnknown)
      return Response::Error("Unknown connection type");
  }
  if (offline || latency || download_throughput || upload_throughput)
    GetNetworkStateNotifier().SetNetworkConnectionInfoOverride(
        !offline, type, download_throughput / (1024 * 1024 / 8));
  else
    GetNetworkStateNotifier().ClearOverride();
  return Response::OK();
}
