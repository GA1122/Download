void MojoAudioOutputStream::Pause() {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  delegate_->OnPauseStream();
}
