void WtsSessionProcessDelegate::Core::Stop() {
  DCHECK(main_task_runner_->BelongsToCurrentThread());

  if (!stopping_) {
    stopping_ = true;

    DrainJobNotificationsCompleted();
  }
}
