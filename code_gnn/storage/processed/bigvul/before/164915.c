int ResourceDispatcherHostImpl::CalculateApproximateMemoryCost(
    net::URLRequest* request) {
  int strings_cost = 0;
  for (net::HttpRequestHeaders::Iterator it(request->extra_request_headers());
       it.GetNext();) {
    strings_cost += it.name().length() + it.value().length();
  }
  strings_cost +=
      request->original_url().parsed_for_possibly_invalid_spec().Length() +
      request->referrer().size() + request->method().size();

  return kAvgBytesPerOutstandingRequest + strings_cost;
}
