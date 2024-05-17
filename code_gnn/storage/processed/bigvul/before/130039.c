bool VariationsService::CreateTrialsFromSeed() {
  DCHECK(thread_checker_.CalledOnValidThread());

  create_trials_from_seed_called_ = true;

  variations::VariationsSeed seed;
  if (!seed_store_.LoadSeed(&seed))
    return false;

  const base::Version current_version(version_info::GetVersionNumber());
  if (!current_version.IsValid())
    return false;

  variations::Study_Channel channel = GetChannelForVariations();
  UMA_HISTOGRAM_SPARSE_SLOWLY("Variations.UserChannel", channel);

  const std::string latest_country =
      local_state_->GetString(prefs::kVariationsCountry);
  variations::VariationsSeedProcessor().CreateTrialsFromSeed(
      seed, g_browser_process->GetApplicationLocale(),
      GetReferenceDateForExpiryChecks(local_state_), current_version, channel,
      GetCurrentFormFactor(), GetHardwareClass(), latest_country,
      LoadPermanentConsistencyCountry(current_version, latest_country),
      base::Bind(&OverrideUIString));

  const base::Time now = base::Time::Now();

  const int64 last_fetch_time_internal =
      local_state_->GetInt64(prefs::kVariationsLastFetchTime);
  if (last_fetch_time_internal) {
    const base::TimeDelta delta =
        now - base::Time::FromInternalValue(last_fetch_time_internal);
    UMA_HISTOGRAM_CUSTOM_COUNTS("Variations.SeedFreshness", delta.InMinutes(),
        1, base::TimeDelta::FromDays(30).InMinutes(), 50);
  }

  const int64 seed_date_internal =
      local_state_->GetInt64(prefs::kVariationsSeedDate);
  if (seed_date_internal) {
    const base::Time seed_date =
        base::Time::FromInternalValue(seed_date_internal);
    const int system_clock_delta_days = (now - seed_date).InDays();
    if (system_clock_delta_days < 0) {
      UMA_HISTOGRAM_COUNTS_100("Variations.SeedDateSkew.SystemClockBehindBy",
                               -system_clock_delta_days);
    } else {
      UMA_HISTOGRAM_COUNTS_100("Variations.SeedDateSkew.SystemClockAheadBy",
                               system_clock_delta_days);
    }

    const int build_time_delta_days =
        (base::GetBuildTime() - seed_date).InDays();
    if (build_time_delta_days < 0) {
      UMA_HISTOGRAM_COUNTS_100("Variations.SeedDateSkew.BuildTimeBehindBy",
                               -build_time_delta_days);
    } else {
      UMA_HISTOGRAM_COUNTS_100("Variations.SeedDateSkew.BuildTimeAheadBy",
                               build_time_delta_days);
    }
  }

  return true;
}
