void ImageLoader::ImageChanged(ImageResourceContent* content,
                               CanDeferInvalidation,
                               const IntRect*) {
  DCHECK_EQ(content, image_content_.Get());
  if (image_complete_ || !content->IsLoading() ||
      delay_until_image_notify_finished_)
    return;

  Document& document = element_->GetDocument();
  if (!document.IsActive())
    return;

  delay_until_image_notify_finished_ =
      IncrementLoadEventDelayCount::Create(document);
}
