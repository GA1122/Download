  virtual net::URLRequestContext* GetRequestContext(
      ResourceType::Type resource_type) {
    net::URLRequestContextGetter* request_context = request_context_;
    if (resource_type == ResourceType::MEDIA)
      request_context = media_request_context_;
    return request_context->GetURLRequestContext();
  }
