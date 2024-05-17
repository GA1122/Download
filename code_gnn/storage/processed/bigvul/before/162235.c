void GamepadProvider::Pause() {
  {
    base::AutoLock lock(is_paused_lock_);
    is_paused_ = true;
  }
  base::MessageLoop* polling_loop = polling_thread_->message_loop();
  polling_loop->task_runner()->PostTask(
      FROM_HERE,
      base::Bind(&GamepadProvider::SendPauseHint, Unretained(this), true));
}
