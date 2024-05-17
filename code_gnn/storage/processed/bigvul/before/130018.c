VariationsSeedDateChangeState GetSeedDateChangeState(
    const base::Time& server_seed_date,
    const base::Time& stored_seed_date) {
  if (server_seed_date < stored_seed_date)
    return SEED_DATE_NEW_DATE_OLDER;

  if (TruncateToUTCDay(server_seed_date) !=
      TruncateToUTCDay(stored_seed_date)) {
    return SEED_DATE_NEW_DAY;
  }
  return SEED_DATE_SAME_DAY;
}
