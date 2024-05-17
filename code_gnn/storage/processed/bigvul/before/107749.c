void FrameLoader::DispatchUnloadEvent() {
  FrameNavigationDisabler navigation_disabler(*frame_);

  protect_provisional_loader_ = false;
  SaveScrollState();

  Document* document = frame_->GetDocument();
  if (document && !SVGImage::IsInSVGImage(document)) {
    document->DispatchUnloadEvents(
        provisional_document_loader_
            ? &provisional_document_loader_->GetTiming()
            : nullptr);
    bool keep_event_listeners =
        provisional_document_loader_ &&
        ShouldReuseDefaultView(
            provisional_document_loader_->Url(),
            provisional_document_loader_->GetContentSecurityPolicy());
    if (!keep_event_listeners)
      document->RemoveAllEventListenersRecursively();
  }
}
