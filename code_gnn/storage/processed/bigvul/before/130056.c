void VariationsService::OnURLFetchComplete(const net::URLFetcher* source) {
  DCHECK(thread_checker_.CalledOnValidThread());
  DCHECK_EQ(pending_seed_request_.get(), source);

  const bool is_first_request = !initial_request_completed_;
  initial_request_completed_ = true;

  scoped_ptr<const net::URLFetcher> request(pending_seed_request_.release());
  const net::URLRequestStatus& request_status = request->GetStatus();
  if (request_status.status() != net::URLRequestStatus::SUCCESS) {
    UMA_HISTOGRAM_SPARSE_SLOWLY("Variations.FailedRequestErrorCode",
                                -request_status.error());
    DVLOG(1) << "Variations server request failed with error: "
             << request_status.error() << ": "
             << net::ErrorToString(request_status.error());
    if (is_first_request)
      request_scheduler_->ScheduleFetchShortly();
    return;
  }

  const int response_code = request->GetResponseCode();
  UMA_HISTOGRAM_SPARSE_SLOWLY("Variations.SeedFetchResponseCode",
                              response_code);

  const base::TimeDelta latency =
      base::TimeTicks::Now() - last_request_started_time_;

  base::Time response_date;
  if (response_code == net::HTTP_OK ||
      response_code == net::HTTP_NOT_MODIFIED) {
    bool success = request->GetResponseHeaders()->GetDateValue(&response_date);
    DCHECK(success || response_date.is_null());

    if (!response_date.is_null()) {
      g_browser_process->network_time_tracker()->UpdateNetworkTime(
          response_date,
          base::TimeDelta::FromMilliseconds(kServerTimeResolutionMs),
          latency,
          base::TimeTicks::Now());
    }
  }

  if (response_code != net::HTTP_OK) {
    DVLOG(1) << "Variations server request returned non-HTTP_OK response code: "
             << response_code;
    if (response_code == net::HTTP_NOT_MODIFIED) {
      RecordLastFetchTime();
      seed_store_.UpdateSeedDateAndLogDayChange(response_date);
    }
    return;
  }

  std::string seed_data;
  bool success = request->GetResponseAsString(&seed_data);
  DCHECK(success);

  net::HttpResponseHeaders* headers = request->GetResponseHeaders();
  const std::string signature = GetHeaderValue(headers, "X-Seed-Signature");
  const std::string country_code = GetHeaderValue(headers, "X-Country");
  const bool is_delta_compressed = (GetHeaderValue(headers, "IM") == "x-bm");
  const bool store_success = StoreSeed(seed_data, signature, country_code,
                                       response_date, is_delta_compressed);
  if (!store_success && is_delta_compressed) {
    disable_deltas_for_next_request_ = true;
    request_scheduler_->ScheduleFetchShortly();
  }
}
