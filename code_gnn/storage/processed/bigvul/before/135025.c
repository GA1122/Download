void AppCacheUpdateJob::URLFetcher::OnReceivedRedirect(
    net::URLRequest* request,
    const net::RedirectInfo& redirect_info,
    bool* defer_redirect) {
  DCHECK_EQ(request_.get(), request);
  job_->MadeProgress();
  redirect_response_code_ = request->GetResponseCode();
  request->Cancel();
  result_ = REDIRECT_ERROR;
  OnResponseCompleted();
}
