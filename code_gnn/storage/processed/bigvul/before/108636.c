  virtual void SetupContext(URLRequestContext* context) {
    context->set_ssl_config_service(
        new TestSSLConfigService(true  ,
                                 true  ));
  }
