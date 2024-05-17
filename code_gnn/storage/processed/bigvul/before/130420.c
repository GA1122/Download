void ThreadWatcher::PostPingMessage() {
  DCHECK(WatchDogThread::CurrentlyOnWatchDogThread());
  if (!active_ || ping_count_ <= 0)
    return;

  ping_time_ = base::TimeTicks::Now();

  base::Closure callback(
      base::Bind(&ThreadWatcher::OnPongMessage, weak_ptr_factory_.GetWeakPtr(),
                 ping_sequence_number_));
  if (watched_runner_->PostTask(
          FROM_HERE,
          base::Bind(&ThreadWatcher::OnPingMessage, thread_id_,
                     callback))) {
      base::MessageLoop::current()->PostDelayedTask(
          FROM_HERE,
          base::Bind(&ThreadWatcher::OnCheckResponsiveness,
                     weak_ptr_factory_.GetWeakPtr(), ping_sequence_number_),
          unresponsive_time_);
  } else {
    DeActivateThreadWatching();
  }
}
