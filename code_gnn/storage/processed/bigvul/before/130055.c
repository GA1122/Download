void VariationsService::OnResourceRequestsAllowed() {
  DCHECK(thread_checker_.CalledOnValidThread());

  RecordRequestsAllowedHistogram(RESOURCE_REQUESTS_ALLOWED_NOTIFIED);
  DVLOG(1) << "Retrying fetch.";
  DoActualFetch();

  DCHECK(request_scheduler_.get());
  request_scheduler_->Reset();
}
