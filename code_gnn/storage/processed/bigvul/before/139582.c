bool RenderMediaClient::IsKeySystemsUpdateNeeded() {
  DVLOG(2) << __func__;
  DCHECK(thread_checker_.CalledOnValidThread());

  if (!has_updated_) {
    DCHECK(is_update_needed_);
    return true;
  }

  if (!is_update_needed_)
    return false;

  static const int kMinUpdateIntervalInMilliseconds = 1000;
  if ((tick_clock_->NowTicks() - last_update_time_ticks_).InMilliseconds() <
      kMinUpdateIntervalInMilliseconds) {
    return false;
  }

  return true;
}
