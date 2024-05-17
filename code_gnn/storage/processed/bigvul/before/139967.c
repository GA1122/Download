void HTMLMediaElement::didMoveToNewDocument(Document& oldDocument) {
  BLINK_MEDIA_LOG << "didMoveToNewDocument(" << (void*)this << ")";

  m_loadTimer.moveToNewTaskRunner(
      TaskRunnerHelper::get(TaskType::Unthrottled, &document()));
  m_progressEventTimer.moveToNewTaskRunner(
      TaskRunnerHelper::get(TaskType::Unthrottled, &document()));
  m_playbackProgressTimer.moveToNewTaskRunner(
      TaskRunnerHelper::get(TaskType::Unthrottled, &document()));
  m_audioTracksTimer.moveToNewTaskRunner(
      TaskRunnerHelper::get(TaskType::Unthrottled, &document()));
  m_viewportFillDebouncerTimer.moveToNewTaskRunner(
      TaskRunnerHelper::get(TaskType::Unthrottled, &document()));
  m_checkViewportIntersectionTimer.moveToNewTaskRunner(
      TaskRunnerHelper::get(TaskType::Unthrottled, &document()));

  m_autoplayUmaHelper->didMoveToNewDocument(oldDocument);
  bool oldDocumentRequiresUserGesture =
      computeLockedPendingUserGesture(oldDocument);
  bool newDocumentRequiresUserGesture =
      computeLockedPendingUserGesture(document());
  if (newDocumentRequiresUserGesture && !oldDocumentRequiresUserGesture)
    m_lockedPendingUserGesture = true;

  if (m_shouldDelayLoadEvent) {
    document().incrementLoadEventDelayCount();
  } else {
    oldDocument.incrementLoadEventDelayCount();
  }

  if (isDocumentCrossOrigin(document()) && !isDocumentCrossOrigin(oldDocument))
    m_lockedPendingUserGestureIfCrossOriginExperimentEnabled = true;

  removeElementFromDocumentMap(this, &oldDocument);
  addElementToDocumentMap(this, &document());

  m_ignorePreloadNone = false;
  invokeLoadAlgorithm();

  oldDocument.decrementLoadEventDelayCount();

  SuspendableObject::didMoveToNewExecutionContext(&document());
  HTMLElement::didMoveToNewDocument(oldDocument);
}
