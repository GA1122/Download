void ImageLoader::UpdateLayoutObject() {
  LayoutImageResource* image_resource = GetLayoutImageResource();

  if (!image_resource)
    return;

  ImageResourceContent* cached_image_content = image_resource->CachedImage();
  if (image_content_ != cached_image_content &&
      (image_complete_ || !cached_image_content))
    image_resource->SetImageResource(image_content_.Get());
}
