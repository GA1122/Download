int GetQuicMaxIdleTimeBeforeCryptoHandshakeSeconds(
    const VariationParameters& quic_trial_params) {
  int value;
  if (base::StringToInt(
          GetVariationParam(quic_trial_params,
                            "max_idle_time_before_crypto_handshake_seconds"),
          &value)) {
    return value;
  }
  return 0;
}
