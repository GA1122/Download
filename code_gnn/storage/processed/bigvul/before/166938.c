void BaseAudioContext::ResolvePromisesForUnpause() {
  DCHECK(IsAudioThread());
  DCHECK(IsGraphOwner());

  if (!is_resolving_resume_promises_ && resume_resolvers_.size() > 0) {
    is_resolving_resume_promises_ = true;
    ScheduleMainThreadCleanup();
  }
}
