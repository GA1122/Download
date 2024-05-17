void MojoAudioOutputIPC::OnError() {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  DCHECK(delegate_);
  delegate_->OnError();
}
