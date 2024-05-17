STDMETHODIMP UrlmonUrlRequest::OnStopBinding(HRESULT result, LPCWSTR error) {
  DCHECK_EQ(thread_, base::PlatformThread::CurrentId());
  DVLOG(1) << __FUNCTION__ << me()
           << "- Request stopped, Result: " << std::hex << result;
  DCHECK(status_.get_state() == Status::WORKING ||
         status_.get_state() == Status::ABORTING);

  Status::State state = status_.get_state();

  status_.Done();

  if (result == INET_E_TERMINATED_BIND) {
    if (terminate_requested()) {
      terminate_bind_callback_->Run(moniker_, bind_context_, upload_data_,
                                    request_headers_.c_str());
    } else {
      cleanup_transaction_ = true;
    }
    result = S_OK;
  }

  if (state == Status::WORKING) {
    status_.set_result(result);

    if (result == E_ACCESSDENIED) {
      int http_code = GetHttpResponseStatusFromBinding(binding_);
      if (300 <= http_code && http_code < 400) {
        status_.set_result(net::URLRequestStatus::FAILED,
                           net::ERR_UNSAFE_REDIRECT);
      }
    }


    if (pending_data_) {
      DCHECK_EQ(pending_read_size_, 0UL);
      ReleaseBindings();
      return S_OK;
    }

    if (headers_received_ && pending_read_size_ == 0) {
      ReleaseBindings();
      return S_OK;
    }

    NotifyDelegateAndDie();
    return S_OK;
  }

  if (status_.was_redirected()) {
    if (!pending_) {
      std::string headers = GetHttpHeadersFromBinding(binding_);
      OnResponse(0, UTF8ToWide(headers).c_str(), NULL, NULL);
    }
    ReleaseBindings();
    return S_OK;
  }

  NotifyDelegateAndDie();
  return S_OK;
}
