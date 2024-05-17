bool VariationsService::StoreSeed(const std::string& seed_data,
                                  const std::string& seed_signature,
                                  const std::string& country_code,
                                  const base::Time& date_fetched,
                                  bool is_delta_compressed) {
  DCHECK(thread_checker_.CalledOnValidThread());

  scoped_ptr<variations::VariationsSeed> seed(new variations::VariationsSeed);
  if (!seed_store_.StoreSeedData(seed_data, seed_signature, country_code,
                                 date_fetched, is_delta_compressed,
                                 seed.get())) {
    return false;
  }
  RecordLastFetchTime();

  if (!state_manager_)
    return true;

  base::PostTaskAndReplyWithResult(
      content::BrowserThread::GetBlockingPool(),
      FROM_HERE,
      base::Bind(&GetVersionForSimulation),
      base::Bind(&VariationsService::PerformSimulationWithVersion,
                 weak_ptr_factory_.GetWeakPtr(), base::Passed(&seed)));
  return true;
}
