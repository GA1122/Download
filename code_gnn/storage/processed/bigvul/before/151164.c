void InspectorNetworkAgent::DidFinishLoading(unsigned long identifier,
                                             DocumentLoader*,
                                             double monotonic_finish_time,
                                             int64_t encoded_data_length,
                                             int64_t decoded_body_length) {
  String request_id = IdentifiersFactory::RequestId(identifier);
  NetworkResourcesData::ResourceData const* resource_data =
      resources_data_->Data(request_id);

  int pending_encoded_data_length =
      resources_data_->GetAndClearPendingEncodedDataLength(request_id);
  if (pending_encoded_data_length > 0) {
    GetFrontend()->dataReceived(request_id, MonotonicallyIncreasingTime(), 0,
                                pending_encoded_data_length);
  }

  if (resource_data &&
      (!resource_data->CachedResource() ||
       resource_data->CachedResource()->GetDataBufferingPolicy() ==
           kDoNotBufferData ||
       IsErrorStatusCode(resource_data->HttpStatusCode()))) {
    resources_data_->MaybeAddResourceData(request_id, "", 0);
  }

  resources_data_->MaybeDecodeDataToContent(request_id);
  if (!monotonic_finish_time)
    monotonic_finish_time = MonotonicallyIncreasingTime();
  GetFrontend()->loadingFinished(request_id, monotonic_finish_time,
                                 encoded_data_length);
}
