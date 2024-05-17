  void FollowRedirect(
      const base::Optional<net::HttpRequestHeaders>& modified_request_headers) {
    DCHECK_CURRENTLY_ON(BrowserThread::IO);
    DCHECK(!redirect_info_.new_url.is_empty());

    if (!IsLoaderInterceptionEnabled()) {
      url_loader_->FollowRedirect(modified_request_headers);
      return;
    }


    bool should_clear_upload = false;
    net::RedirectUtil::UpdateHttpRequest(
        resource_request_->url, resource_request_->method, redirect_info_,
        modified_request_headers, &resource_request_->headers,
        &should_clear_upload);
    if (should_clear_upload) {
      resource_request_->request_body = nullptr;
      blob_handles_.clear();
    }

    resource_request_->url = redirect_info_.new_url;
    resource_request_->method = redirect_info_.new_method;
    resource_request_->site_for_cookies = redirect_info_.new_site_for_cookies;
    resource_request_->referrer = GURL(redirect_info_.new_referrer);
    resource_request_->referrer_policy = redirect_info_.new_referrer_policy;
    url_chain_.push_back(redirect_info_.new_url);

    url_loader_modified_request_headers_ = modified_request_headers;

    if (signed_exchange_utils::NeedToCheckRedirectedURLForAcceptHeader()) {
      if (!url_loader_modified_request_headers_)
        url_loader_modified_request_headers_ = net::HttpRequestHeaders();
      std::string accept_value = network::kFrameAcceptHeader;
      if (signed_exchange_utils::ShouldAdvertiseAcceptHeader(
              url::Origin::Create(resource_request_->url))) {
        DCHECK(!accept_value.empty());
        accept_value.append(kAcceptHeaderSignedExchangeSuffix);
      }
      url_loader_modified_request_headers_->SetHeader(network::kAcceptHeader,
                                                      accept_value);
      resource_request_->headers.SetHeader(network::kAcceptHeader,
                                           accept_value);
    }

    Restart();
  }
