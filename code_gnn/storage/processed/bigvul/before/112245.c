void UrlFetcher::Core::Detach() {
  DCHECK(delegate_message_loop_->BelongsToCurrentThread());
  network_task_runner_->PostTask(
      FROM_HERE, base::Bind(&UrlFetcher::Core::CancelRequest, this));
  done_callback_.Reset();
}
