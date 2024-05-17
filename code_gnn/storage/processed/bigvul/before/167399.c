  ~ClearReportingCacheTester() {
    net::URLRequestContext* request_context =
        profile_->GetRequestContext()->GetURLRequestContext();
    DCHECK_EQ(service_.get(), request_context->reporting_service());
    request_context->set_reporting_service(old_service_);
  }
