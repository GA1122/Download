  virtual URLRequestJob* MaybeInterceptRedirect(
      URLRequest* request,
      NetworkDelegate* network_delegate,
      const GURL& location) {
    if (cancel_redirect_request_) {
      cancel_redirect_request_ = false;
      did_cancel_redirect_ = true;
      return new CancelTestJob(request, network_delegate);
    }
    if (!intercept_redirect_)
      return NULL;
    intercept_redirect_ = false;
    did_intercept_redirect_ = true;
    return new URLRequestTestJob(request,
                                 network_delegate,
                                 redirect_headers_,
                                 redirect_data_,
                                 true);
  }
