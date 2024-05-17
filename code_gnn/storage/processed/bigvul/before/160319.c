void ImageLoader::SetImageWithoutConsideringPendingLoadEvent(
    ImageResourceContent* new_image_content) {
  DCHECK(failed_load_url_.IsEmpty());
  ImageResourceContent* old_image_content = image_content_.Get();
  if (new_image_content != old_image_content) {
    if (pending_load_event_.IsActive())
      pending_load_event_.Cancel();
    if (pending_error_event_.IsActive())
      pending_error_event_.Cancel();
    UpdateImageState(new_image_content);
    if (new_image_content) {
      new_image_content->AddObserver(this);
    }
    if (old_image_content) {
      old_image_content->RemoveObserver(this);
    }
  }

  if (LayoutImageResource* image_resource = GetLayoutImageResource())
    image_resource->ResetAnimation();
}
