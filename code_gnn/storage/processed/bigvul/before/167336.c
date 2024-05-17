  ClearNetworkErrorLoggingTester(TestingProfile* profile, bool create_service)
      : profile_(profile) {
    if (create_service)
      delegate_ = std::make_unique<MockNetworkErrorLoggingDelegate>();

    net::URLRequestContext* request_context =
        profile_->GetRequestContext()->GetURLRequestContext();

    request_context->set_network_error_logging_delegate(delegate_.get());
  }
