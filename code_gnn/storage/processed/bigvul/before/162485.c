void ImageResource::Trace(blink::Visitor* visitor) {
  visitor->Trace(multipart_parser_);
  visitor->Trace(content_);
  Resource::Trace(visitor);
  MultipartImageResourceParser::Client::Trace(visitor);
}
