void VariationsService::OnAppEnterForeground() {
  DCHECK(thread_checker_.CalledOnValidThread());

  if (!request_scheduler_)
    StartRepeatedVariationsSeedFetch();
  request_scheduler_->OnAppEnterForeground();
}
