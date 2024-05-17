void GamepadProvider::Resume() {
  {
    base::AutoLock lock(is_paused_lock_);
    if (!is_paused_)
      return;
    is_paused_ = false;
  }

  base::MessageLoop* polling_loop = polling_thread_->message_loop();
  polling_loop->task_runner()->PostTask(
      FROM_HERE,
      base::Bind(&GamepadProvider::SendPauseHint, Unretained(this), false));
  polling_loop->task_runner()->PostTask(
      FROM_HERE,
      base::Bind(&GamepadProvider::ScheduleDoPoll, Unretained(this)));
}
