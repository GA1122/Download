void Document::LoadEventDelayTimerFired(TimerBase*) {
  CheckCompleted();
}
