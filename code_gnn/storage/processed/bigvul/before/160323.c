void ImageLoader::Trace(blink::Visitor* visitor) {
  visitor->Trace(image_content_);
  visitor->Trace(image_resource_for_image_document_);
  visitor->Trace(element_);
  visitor->Trace(decode_requests_);
}
