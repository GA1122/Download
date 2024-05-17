int ResourceDispatcherHost::CalculateApproximateMemoryCost(
    URLRequest* request) {
  int strings_cost = request->extra_request_headers().size() +
                     request->original_url().spec().size() +
                     request->referrer().size() +
                     request->method().size();

  int upload_cost = 0;

#if 0
  if (request->has_upload()) {
    const std::vector<net::UploadData::Element>& uploads =
        request->get_upload()->elements();
    std::vector<net::UploadData::Element>::const_iterator iter;
    for (iter = uploads.begin(); iter != uploads.end(); ++iter) {
      if (iter->type() == net::UploadData::TYPE_BYTES) {
        int64 element_size = iter->GetContentLength();
        upload_cost += static_cast<int>(element_size);
      }
    }
  }
#endif

  return kAvgBytesPerOutstandingRequest + strings_cost + upload_cost;
}
