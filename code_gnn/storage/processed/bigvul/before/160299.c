void ImageLoader::DispatchPendingLoadEvent(
    std::unique_ptr<IncrementLoadEventDelayCount> count) {
  if (!image_content_)
    return;
  CHECK(image_complete_);
  if (GetElement()->GetDocument().GetFrame())
    DispatchLoadEvent();

  count->ClearAndCheckLoadEvent();
}
