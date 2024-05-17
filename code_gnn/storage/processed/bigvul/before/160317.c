void ImageLoader::SetImageForImageDocument(ImageResource* new_image_resource) {
  DCHECK(loading_image_document_);
  DCHECK(new_image_resource);
  DCHECK(new_image_resource->GetContent());

  image_resource_for_image_document_ = new_image_resource;
  SetImageWithoutConsideringPendingLoadEvent(new_image_resource->GetContent());

  image_complete_ = true;
}
