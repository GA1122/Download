void AppCacheUpdateJob::URLFetcher::OnResponseStarted(
    net::URLRequest *request) {
  DCHECK(request == request_);
  int response_code = -1;
  if (request->status().is_success()) {
    response_code = request->GetResponseCode();
    job_->MadeProgress();
  }

  if ((response_code / 100) != 2) {
    if (response_code > 0)
      result_ = SERVER_ERROR;
    else
      result_ = NETWORK_ERROR;
    OnResponseCompleted();
    return;
  }

  if (url_.SchemeIsCryptographic()) {
    const net::HttpNetworkSession::Params* session_params =
        request->context()->GetNetworkSessionParams();
    bool ignore_cert_errors = session_params &&
                              session_params->ignore_certificate_errors;
    if ((net::IsCertStatusError(request->ssl_info().cert_status) &&
            !ignore_cert_errors) ||
        (url_.GetOrigin() != job_->manifest_url_.GetOrigin() &&
            request->response_headers()->
                HasHeaderValue("cache-control", "no-store"))) {
      DCHECK_EQ(-1, redirect_response_code_);
      request->Cancel();
      result_ = SECURITY_ERROR;
      OnResponseCompleted();
      return;
    }
  }

  if (fetch_type_ == URL_FETCH || fetch_type_ == MASTER_ENTRY_FETCH) {
    response_writer_.reset(job_->CreateResponseWriter());
    scoped_refptr<HttpResponseInfoIOBuffer> io_buffer(
        new HttpResponseInfoIOBuffer(
            new net::HttpResponseInfo(request->response_info())));
    response_writer_->WriteInfo(
        io_buffer.get(),
        base::Bind(&URLFetcher::OnWriteComplete, base::Unretained(this)));
  } else {
    ReadResponseData();
  }
}
