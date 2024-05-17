void ThreadWatcher::OnPongMessage(uint64 ping_sequence_number) {
  DCHECK(WatchDogThread::CurrentlyOnWatchDogThread());

  base::TimeTicks now = base::TimeTicks::Now();
  base::TimeDelta response_time = now - ping_time_;
  response_time_histogram_->AddTime(response_time);

  pong_time_ = now;

  DCHECK_EQ(ping_sequence_number_, ping_sequence_number);
  if (ping_sequence_number_ != ping_sequence_number)
    return;

  ++ping_sequence_number_;

  if (!active_ || --ping_count_ <= 0)
    return;

  base::ThreadTaskRunnerHandle::Get()->PostDelayedTask(
      FROM_HERE, base::Bind(&ThreadWatcher::PostPingMessage,
                            weak_ptr_factory_.GetWeakPtr()),
      sleep_time_);
}
