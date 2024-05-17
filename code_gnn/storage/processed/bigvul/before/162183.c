void MojoAudioOutputIPC::SetVolume(double volume) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  DCHECK(stream_.is_bound());
  stream_->SetVolume(volume);
}
