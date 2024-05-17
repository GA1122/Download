void VariationsService::DoActualFetch() {
  DCHECK(thread_checker_.CalledOnValidThread());
  DCHECK(!pending_seed_request_);

  pending_seed_request_ = net::URLFetcher::Create(0, variations_server_url_,
                                                  net::URLFetcher::GET, this);
  pending_seed_request_->SetLoadFlags(net::LOAD_DO_NOT_SEND_COOKIES |
                                      net::LOAD_DO_NOT_SAVE_COOKIES);
  pending_seed_request_->SetRequestContext(
      g_browser_process->system_request_context());
  pending_seed_request_->SetMaxRetriesOn5xx(kMaxRetrySeedFetch);
  if (!seed_store_.variations_serial_number().empty() &&
      !disable_deltas_for_next_request_) {
    if (!seed_store_.seed_has_country_code()) {
      pending_seed_request_->AddExtraRequestHeader("A-IM:x-bm");
    }
    pending_seed_request_->AddExtraRequestHeader(
        "If-None-Match:" + seed_store_.variations_serial_number());
  }
  pending_seed_request_->Start();

  const base::TimeTicks now = base::TimeTicks::Now();
  base::TimeDelta time_since_last_fetch;
  if (!last_request_started_time_.is_null())
    time_since_last_fetch = now - last_request_started_time_;
  UMA_HISTOGRAM_CUSTOM_COUNTS("Variations.TimeSinceLastFetchAttempt",
                              time_since_last_fetch.InMinutes(), 0,
                              base::TimeDelta::FromDays(7).InMinutes(), 50);
  UMA_HISTOGRAM_COUNTS_100("Variations.RequestCount", request_count_);
  ++request_count_;
  last_request_started_time_ = now;
  disable_deltas_for_next_request_ = false;
}
