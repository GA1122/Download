void ImageLoader::DispatchPendingErrorEvent(
    std::unique_ptr<IncrementLoadEventDelayCount> count) {
  if (GetElement()->GetDocument().GetFrame())
    GetElement()->DispatchEvent(Event::Create(EventTypeNames::error));

  count->ClearAndCheckLoadEvent();
}
