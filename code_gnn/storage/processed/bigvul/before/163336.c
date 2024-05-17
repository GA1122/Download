RenderThreadImpl::GetMediaThreadTaskRunner() {
  DCHECK(message_loop()->task_runner()->BelongsToCurrentThread());
  if (!media_thread_) {
    media_thread_.reset(new base::Thread("Media"));
    media_thread_->Start();
  }
  return media_thread_->task_runner();
}
