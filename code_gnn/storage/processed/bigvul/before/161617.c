void AudioHandler::EnableOutputsIfNecessary() {
  if (is_disabled_ && connection_ref_count_ > 0) {
    DCHECK(IsMainThread());
    BaseAudioContext::GraphAutoLocker locker(Context());

    is_disabled_ = false;
    for (auto& output : outputs_)
      output->Enable();
  }
}
