void WtsSessionProcessDelegate::Core::DrainJobNotificationsCompleted() {
  DCHECK(main_task_runner_->BelongsToCurrentThread());

  if (job_.IsValid()) {
    job_.Close();

    io_task_runner_->PostTask(FROM_HERE, base::Bind(
        &Core::DrainJobNotifications, this));
  }
}
