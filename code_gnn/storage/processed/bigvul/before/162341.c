void MojoAudioInputStream::OnMuted(int stream_id, bool is_muted) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  client_->OnMutedStateChanged(is_muted);
}
