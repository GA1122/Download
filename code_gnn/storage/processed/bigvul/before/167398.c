  ~ClearNetworkErrorLoggingTester() {
    net::URLRequestContext* request_context =
        profile_->GetRequestContext()->GetURLRequestContext();
    DCHECK_EQ(delegate_.get(),
              request_context->network_error_logging_delegate());
    request_context->set_network_error_logging_delegate(nullptr);
  }
