bool ResourceMultiBufferDataProvider::VerifyPartialResponse(
    const WebURLResponse& response,
    const scoped_refptr<UrlData>& url_data) {
  int64_t first_byte_position, last_byte_position, instance_size;
  if (!ParseContentRange(response.HttpHeaderField("Content-Range").Utf8(),
                         &first_byte_position, &last_byte_position,
                         &instance_size)) {
    return false;
  }

  if (url_data_->length() == kPositionNotSpecified) {
    url_data->set_length(instance_size);
  }

  if (first_byte_position > byte_pos()) {
    return false;
  }
  if (last_byte_position + 1 < byte_pos()) {
    return false;
  }
  bytes_to_discard_ = byte_pos() - first_byte_position;

  return true;
}
