void VariationsService::RecordLastFetchTime() {
  DCHECK(thread_checker_.CalledOnValidThread());

  if (local_state_) {
    local_state_->SetInt64(prefs::kVariationsLastFetchTime,
                           base::Time::Now().ToInternalValue());
  }
}
