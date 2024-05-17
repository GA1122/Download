bool Document::ShouldComplete() {
  return parsing_state_ == kFinishedParsing && HaveImportsLoaded() &&
         !fetcher_->BlockingRequestCount() && !IsDelayingLoadEvent() &&
         load_event_progress_ != kLoadEventInProgress &&
         AllDescendantsAreComplete(frame_);
}
