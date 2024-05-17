void HTMLMediaElement::DidMoveToNewDocument(Document& old_document) {
  BLINK_MEDIA_LOG << "didMoveToNewDocument(" << (void*)this << ")";

  load_timer_.MoveToNewTaskRunner(
      GetDocument().GetTaskRunner(TaskType::kUnthrottled));
  progress_event_timer_.MoveToNewTaskRunner(
      GetDocument().GetTaskRunner(TaskType::kUnthrottled));
  playback_progress_timer_.MoveToNewTaskRunner(
      GetDocument().GetTaskRunner(TaskType::kUnthrottled));
  audio_tracks_timer_.MoveToNewTaskRunner(
      GetDocument().GetTaskRunner(TaskType::kUnthrottled));
  check_viewport_intersection_timer_.MoveToNewTaskRunner(
      GetDocument().GetTaskRunner(TaskType::kUnthrottled));
  deferred_load_timer_.MoveToNewTaskRunner(
      GetDocument().GetTaskRunner(TaskType::kUnthrottled));

  autoplay_policy_->DidMoveToNewDocument(old_document);

  if (should_delay_load_event_) {
    GetDocument().IncrementLoadEventDelayCount();
  } else {
    old_document.IncrementLoadEventDelayCount();
  }

  RemoveElementFromDocumentMap(this, &old_document);
  AddElementToDocumentMap(this, &GetDocument());

  ignore_preload_none_ = false;
  InvokeLoadAlgorithm();

  old_document.DecrementLoadEventDelayCount();

  PausableObject::DidMoveToNewExecutionContext(&GetDocument());
  HTMLElement::DidMoveToNewDocument(old_document);
}
