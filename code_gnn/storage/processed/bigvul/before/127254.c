void WtsSessionProcessDelegate::Core::DrainJobNotifications() {
  DCHECK(io_task_runner_->BelongsToCurrentThread());

  main_task_runner_->PostTask(FROM_HERE, base::Bind(
      &Core::DrainJobNotificationsCompleted, this));
}
