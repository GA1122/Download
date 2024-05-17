void CastStreamingNativeHandler::Invalidate() {
  weak_factory_.InvalidateWeakPtrs();

  get_stats_callbacks_.clear();
  get_raw_events_callbacks_.clear();
  create_callback_.Reset();
  udp_transport_map_.clear();
  rtp_stream_map_.clear();

  ObjectBackedNativeHandler::Invalidate();
}
