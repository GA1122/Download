void HTMLMediaElement::OnRemovedFromDocumentTimerFired(TimerBase*) {
  if (InActiveDocument())
    return;

  PauseInternal();
}
