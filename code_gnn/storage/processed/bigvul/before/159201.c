bool DownloadItemImpl::TimeRemaining(base::TimeDelta* remaining) const {
  if (total_bytes_ <= 0)
    return false;   

  int64_t speed = CurrentSpeed();
  if (speed == 0)
    return false;

  *remaining =
      base::TimeDelta::FromSeconds((total_bytes_ - GetReceivedBytes()) / speed);
  return true;
}
