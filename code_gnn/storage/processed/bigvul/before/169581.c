CastRtpStream* CastStreamingNativeHandler::GetRtpStreamOrThrow(
    int transport_id) const {
  RtpStreamMap::const_iterator iter = rtp_stream_map_.find(
      transport_id);
  if (iter != rtp_stream_map_.end())
    return iter->second.get();
  v8::Isolate* isolate = context()->v8_context()->GetIsolate();
  isolate->ThrowException(v8::Exception::RangeError(v8::String::NewFromUtf8(
      isolate, kRtpStreamNotFound)));
  return NULL;
}
