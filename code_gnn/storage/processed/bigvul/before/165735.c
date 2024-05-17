size_t GetQuicMaxPacketLength(const VariationParameters& quic_trial_params) {
  unsigned value;
  if (base::StringToUint(
          GetVariationParam(quic_trial_params, "max_packet_length"), &value)) {
    return value;
  }
  return 0;
}
