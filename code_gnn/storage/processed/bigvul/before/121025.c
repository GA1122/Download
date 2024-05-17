void HttpStreamParser::CalculateResponseBodySize() {

  switch (response_->headers->response_code()) {
    case 204:   
    case 205:   
    case 304:   
      response_body_length_ = 0;
      break;
  }
  if (request_->method == "HEAD")
    response_body_length_ = 0;

  if (response_body_length_ == -1) {
    if (response_->headers->IsChunkEncoded()) {
      chunked_decoder_.reset(new HttpChunkedDecoder());
    } else {
      response_body_length_ = response_->headers->GetContentLength();
    }
  }
}
