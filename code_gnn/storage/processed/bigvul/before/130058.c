void VariationsService::PerformSimulationWithVersion(
    scoped_ptr<variations::VariationsSeed> seed,
    const base::Version& version) {
  DCHECK(thread_checker_.CalledOnValidThread());

  if (!version.IsValid())
    return;

  const base::ElapsedTimer timer;

  scoped_ptr<const base::FieldTrial::EntropyProvider> entropy_provider =
      state_manager_->CreateEntropyProvider();
  variations::VariationsSeedSimulator seed_simulator(*entropy_provider);

  const std::string latest_country =
      local_state_->GetString(prefs::kVariationsCountry);
  const variations::VariationsSeedSimulator::Result result =
      seed_simulator.SimulateSeedStudies(
          *seed, g_browser_process->GetApplicationLocale(),
          GetReferenceDateForExpiryChecks(local_state_), version,
          GetChannelForVariations(), GetCurrentFormFactor(), GetHardwareClass(),
          latest_country,
          LoadPermanentConsistencyCountry(version, latest_country));

  UMA_HISTOGRAM_COUNTS_100("Variations.SimulateSeed.NormalChanges",
                           result.normal_group_change_count);
  UMA_HISTOGRAM_COUNTS_100("Variations.SimulateSeed.KillBestEffortChanges",
                           result.kill_best_effort_group_change_count);
  UMA_HISTOGRAM_COUNTS_100("Variations.SimulateSeed.KillCriticalChanges",
                           result.kill_critical_group_change_count);

  UMA_HISTOGRAM_TIMES("Variations.SimulateSeed.Duration", timer.Elapsed());

  NotifyObservers(result);
}
