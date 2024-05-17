void ResourceLoader::RequestSynchronously(const ResourceRequest& request) {
  DCHECK(!request.DownloadToFile());
  DCHECK(loader_);
  DCHECK_EQ(request.Priority(), kResourceLoadPriorityHighest);

  WrappedResourceRequest request_in(request);
  WebURLResponse response_out;
  WebURLError error_out;
  WebData data_out;
  int64_t encoded_data_length = WebURLLoaderClient::kUnknownEncodedDataLength;
  int64_t encoded_body_length = 0;
  loader_->LoadSynchronously(request_in, response_out, error_out, data_out,
                             encoded_data_length, encoded_body_length);

  if (!loader_)
    return;
  int64_t decoded_body_length = data_out.size();
  if (error_out.reason) {
    DidFail(error_out, encoded_data_length, encoded_body_length,
            decoded_body_length);
    return;
  }
  DidReceiveResponse(response_out);
  if (!loader_)
    return;
  DCHECK_GE(response_out.ToResourceResponse().EncodedBodyLength(), 0);

  if (data_out.size()) {
    data_out.ForEachSegment([this](const char* segment, size_t segment_size,
                                   size_t segment_offset) {
      Context().DispatchDidReceiveData(resource_->Identifier(), segment,
                                       segment_size);
      return true;
    });
    resource_->SetResourceBuffer(data_out);
  }
  DidFinishLoading(MonotonicallyIncreasingTime(), encoded_data_length,
                   encoded_body_length, decoded_body_length);
}
