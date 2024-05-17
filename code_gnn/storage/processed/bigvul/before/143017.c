AudioIOPosition BaseAudioContext::OutputPosition() const {
  DCHECK(IsMainThread());
  GraphAutoLocker locker(this);
  return output_position_;
}
