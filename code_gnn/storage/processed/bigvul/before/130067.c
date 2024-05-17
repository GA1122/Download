void VariationsService::StartRepeatedVariationsSeedFetch() {
  DCHECK(thread_checker_.CalledOnValidThread());

  variations_server_url_ =
      GetVariationsServerURL(policy_pref_service_, restrict_mode_);

  DCHECK(create_trials_from_seed_called_);

  DCHECK(!request_scheduler_.get());
  request_scheduler_.reset(VariationsRequestScheduler::Create(
      base::Bind(&VariationsService::FetchVariationsSeed,
                 weak_ptr_factory_.GetWeakPtr()),
      local_state_));
  request_scheduler_->Start();
}
