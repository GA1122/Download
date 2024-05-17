  ClearReportingCacheTester(TestingProfile* profile, bool create_service)
      : profile_(profile) {
    if (create_service)
      service_ = std::make_unique<MockReportingService>();

    net::URLRequestContext* request_context =
        profile_->GetRequestContext()->GetURLRequestContext();
    old_service_ = request_context->reporting_service();
    request_context->set_reporting_service(service_.get());
  }
