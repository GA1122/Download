int GetQuicMaxNumMigrationsToNonDefaultNetworkOnWriteError(
    const VariationParameters& quic_trial_params) {
  int value;
  if (base::StringToInt(
          GetVariationParam(
              quic_trial_params,
              "max_migrations_to_non_default_network_on_write_error"),
          &value)) {
    return value;
  }
  return 0;
}
