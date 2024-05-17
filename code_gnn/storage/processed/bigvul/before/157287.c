void WebMediaPlayerImpl::OnWaitingForDecryptionKey() {
  DCHECK(main_task_runner_->BelongsToCurrentThread());

  encrypted_client_->DidBlockPlaybackWaitingForKey();
  encrypted_client_->DidResumePlaybackBlockedForKey();
}
