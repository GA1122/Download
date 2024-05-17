net::RequestPriority DetermineRequestPriority(
    const ResourceHostMsg_Request& request_data) {
  switch (request_data.priority) {
    case WebKit::WebURLRequest::PriorityVeryHigh:
      return net::HIGHEST;

    case WebKit::WebURLRequest::PriorityHigh:
      return net::MEDIUM;

    case WebKit::WebURLRequest::PriorityMedium:
      return net::LOW;

    case WebKit::WebURLRequest::PriorityLow:
      return net::LOWEST;

    case WebKit::WebURLRequest::PriorityVeryLow:
      return net::IDLE;

    case WebKit::WebURLRequest::PriorityUnresolved:
    default:
      NOTREACHED();
      return net::LOW;
  }
}
