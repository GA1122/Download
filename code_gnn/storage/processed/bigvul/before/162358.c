void MojoAudioOutputStream::SetVolume(double volume) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  if (volume < 0 || volume > 1) {
    LOG(ERROR) << "MojoAudioOutputStream::SetVolume(" << volume
               << ") out of range.";
    OnStreamError(  0);
    return;
  }
  delegate_->OnSetVolume(volume);
}
