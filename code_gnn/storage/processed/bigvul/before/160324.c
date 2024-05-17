void ImageLoader::DecodeRequest::Trace(blink::Visitor* visitor) {
  visitor->Trace(resolver_);
  visitor->Trace(loader_);
}
