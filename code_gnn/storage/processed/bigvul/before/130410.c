JankTimeBomb::JankTimeBomb(base::TimeDelta duration)
    : weak_ptr_factory_(this) {
  if (IsEnabled()) {
    WatchDogThread::PostDelayedTask(
        FROM_HERE,
        base::Bind(&JankTimeBomb::Alarm,
                   weak_ptr_factory_.GetWeakPtr(),
                   base::PlatformThread::CurrentId()),
        duration);
  }
}
