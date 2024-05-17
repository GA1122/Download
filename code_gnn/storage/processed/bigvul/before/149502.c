void RequestSender::SendInternal() {
  DCHECK(cur_url_ != urls_.end());
  DCHECK(cur_url_->is_valid());
  DCHECK(thread_checker_.CalledOnValidThread());

  GURL url(*cur_url_);

  if (use_signing_) {
    DCHECK(!public_key_.empty());
    signer_ = client_update_protocol::Ecdsa::Create(kKeyVersion, public_key_);
    std::string request_query_string;
    signer_->SignRequest(request_body_, &request_query_string);

    url = BuildUpdateUrl(url, request_query_string);
  }

  DVLOG(2) << "Sending Omaha request: " << request_body_;

  network_fetcher_ = config_->GetNetworkFetcherFactory()->Create();
  if (!network_fetcher_) {
    base::ThreadTaskRunnerHandle::Get()->PostTask(
        FROM_HERE,
        base::BindOnce(&RequestSender::SendInternalComplete,
                       base::Unretained(this),
                       static_cast<int>(ProtocolError::URL_FETCHER_FAILED),
                       std::string(), std::string(), 0));
  }
  network_fetcher_->PostRequest(
      url, request_body_, request_extra_headers_,
      base::BindOnce(&RequestSender::OnResponseStarted, base::Unretained(this)),
      base::BindRepeating([](int64_t current) {}),
      base::BindOnce(&RequestSender::OnNetworkFetcherComplete,
                     base::Unretained(this), url));
}
