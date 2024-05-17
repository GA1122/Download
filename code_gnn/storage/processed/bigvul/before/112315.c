int ResourceDispatcherHostImpl::CalculateApproximateMemoryCost(
    net::URLRequest* request) {
  int strings_cost = request->extra_request_headers().ToString().size() +
                     request->original_url().spec().size() +
                     request->referrer().size() +
                     request->method().size();

  return kAvgBytesPerOutstandingRequest + strings_cost;
}
