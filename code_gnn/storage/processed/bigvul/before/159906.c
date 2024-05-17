void TopSitesImpl::StartQueryForMostVisited() {
  DCHECK(loaded_);
  timer_.Stop();

  if (!history_service_)
    return;

  provider_->ProvideTopSites(
      num_results_to_request_from_history(),
      base::Bind(&TopSitesImpl::OnTopSitesAvailableFromHistory,
                 base::Unretained(this)),
      &cancelable_task_tracker_);
}
