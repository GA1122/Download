ResourceMultiBufferDataProvider::ResourceMultiBufferDataProvider(
    UrlData* url_data,
    MultiBufferBlockId pos,
    bool is_client_audio_element)
    : pos_(pos),
      url_data_(url_data),
      retries_(0),
      cors_mode_(url_data->cors_mode()),
      origin_(url_data->url().GetOrigin()),
      is_client_audio_element_(is_client_audio_element),
      weak_factory_(this) {
  DCHECK(url_data_) << " pos = " << pos;
  DCHECK_GE(pos, 0);
}
