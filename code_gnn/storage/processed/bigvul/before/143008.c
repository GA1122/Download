void BaseAudioContext::HandleStoppableSourceNodes() {
  DCHECK(IsAudioThread());
  AssertGraphOwner();

  if (finished_source_handlers_.size())
    ScheduleMainThreadCleanup();
}
