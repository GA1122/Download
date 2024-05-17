NET_EXPORT_PRIVATE std::vector<uint8_t> GenerateUpdatedTargetInfo(
    bool is_mic_enabled,
    bool is_epa_enabled,
    const std::string& channel_bindings,
    const std::string& spn,
    const std::vector<AvPair>& av_pairs,
    uint64_t* server_timestamp) {
  size_t updated_target_info_len = 0;
  std::vector<AvPair> updated_av_pairs(av_pairs);
  UpdateTargetInfoAvPairs(is_mic_enabled, is_epa_enabled, channel_bindings, spn,
                          &updated_av_pairs, server_timestamp,
                          &updated_target_info_len);
  return WriteUpdatedTargetInfo(updated_av_pairs, updated_target_info_len);
}
