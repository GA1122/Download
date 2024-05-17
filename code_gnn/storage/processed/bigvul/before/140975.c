void Document::CheckCompleted() {
  if (CheckCompletedInternal())
    frame_->Loader().DidFinishNavigation();
}
