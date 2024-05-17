base::flat_set<std::string> GetQuicHostWhitelist(
    const VariationParameters& quic_trial_params) {
  std::string host_whitelist =
      GetVariationParam(quic_trial_params, "host_whitelist");
  std::vector<std::string> host_vector = base::SplitString(
      host_whitelist, ",", base::TRIM_WHITESPACE, base::SPLIT_WANT_ALL);
  return base::flat_set<std::string>(std::move(host_vector));
}
