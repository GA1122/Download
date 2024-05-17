size_t GetLocalStreamUsageCount(
    const std::vector<std::unique_ptr<RTCRtpSender>>& rtp_senders,
    const std::string stream_id) {
  size_t usage_count = 0;
  for (const auto& sender : rtp_senders) {
    for (const auto& sender_stream_id : sender->state().stream_ids()) {
      if (sender_stream_id == stream_id) {
        ++usage_count;
        break;
      }
    }
  }
  return usage_count;
}
