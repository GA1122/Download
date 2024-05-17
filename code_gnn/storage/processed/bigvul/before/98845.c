int WebSocketExperimentTask::DoURLFetch() {
  DCHECK(!url_fetcher_.get());

  url_fetcher_.reset(context_->CreateURLFetcher(config_, this));
  if (!url_fetcher_.get()) {
    next_state_ = STATE_NONE;
    return net::ERR_UNEXPECTED;
  }

  next_state_ = STATE_URL_FETCH_COMPLETE;
  SetTimeout(config_.url_fetch_deadline_ms);
  url_fetch_start_time_ = base::TimeTicks::Now();
  url_fetcher_->Start();
  return net::ERR_IO_PENDING;
}
