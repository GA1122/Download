ExecutionContext* TextTrack::GetExecutionContext() const {
  HTMLMediaElement* owner = MediaElement();
  return owner ? owner->GetExecutionContext() : nullptr;
}
