AudioIOPosition BaseAudioContext::OutputPosition() {
  DCHECK(IsMainThread());
  GraphAutoLocker locker(this);
  return output_position_;
}
