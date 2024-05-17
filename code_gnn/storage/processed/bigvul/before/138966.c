base::TimeDelta WallpaperManagerBase::GetWallpaperLoadDelay() const {
  base::TimeDelta delay;

  if (last_load_times_.size() == 0) {
    delay = base::TimeDelta::FromMilliseconds(kLoadDefaultDelayMs);
  } else {
    delay = std::accumulate(last_load_times_.begin(), last_load_times_.end(),
                            base::TimeDelta(), std::plus<base::TimeDelta>()) /
            last_load_times_.size();
  }

  if (delay < base::TimeDelta::FromMilliseconds(kLoadMinDelayMs))
    delay = base::TimeDelta::FromMilliseconds(kLoadMinDelayMs);
  else if (delay > base::TimeDelta::FromMilliseconds(kLoadMaxDelayMs))
    delay = base::TimeDelta::FromMilliseconds(kLoadMaxDelayMs);

  if (!last_load_finished_at_.is_null()) {
    const base::TimeDelta interval = base::Time::Now() - last_load_finished_at_;
    if (interval > delay)
      delay = base::TimeDelta::FromMilliseconds(0);
    else if (interval > base::TimeDelta::FromMilliseconds(0))
      delay -= interval;
  }
  return delay;
}
