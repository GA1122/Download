  virtual void didReceiveResponse(
      WebURLLoader*, const WebURLResponse& response) {
    if (!MultipartResponseDelegate::ReadContentRanges(
            response,
            &byte_range_lower_bound_,
            &byte_range_upper_bound_)) {
      NOTREACHED();
      return;
    }

    resource_response_ = response;
  }
