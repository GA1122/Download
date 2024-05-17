CastUdpTransport* CastStreamingNativeHandler::GetUdpTransportOrThrow(
    int transport_id) const {
  UdpTransportMap::const_iterator iter = udp_transport_map_.find(
      transport_id);
  if (iter != udp_transport_map_.end())
    return iter->second.get();
  v8::Isolate* isolate = context()->v8_context()->GetIsolate();
  isolate->ThrowException(v8::Exception::RangeError(
      v8::String::NewFromUtf8(isolate, kUdpTransportNotFound)));
  return NULL;
}
