void HTMLMediaElement::DidMoveToNewDocument(Document& old_document) {
  BLINK_MEDIA_LOG << "didMoveToNewDocument(" << (void*)this << ")";

  load_timer_.MoveToNewTaskRunner(
      GetDocument().GetTaskRunner(TaskType::kInternalMedia));
  progress_event_timer_.MoveToNewTaskRunner(
      GetDocument().GetTaskRunner(TaskType::kInternalMedia));
  playback_progress_timer_.MoveToNewTaskRunner(
      GetDocument().GetTaskRunner(TaskType::kInternalMedia));
  audio_tracks_timer_.MoveToNewTaskRunner(
      GetDocument().GetTaskRunner(TaskType::kInternalMedia));
  if (viewport_intersection_observer_) {
    ActivateViewportIntersectionMonitoring(false);
    ActivateViewportIntersectionMonitoring(true);
  }
  deferred_load_timer_.MoveToNewTaskRunner(
      GetDocument().GetTaskRunner(TaskType::kInternalMedia));
  removed_from_document_timer_.MoveToNewTaskRunner(
      GetDocument().GetTaskRunner(TaskType::kInternalMedia));

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
