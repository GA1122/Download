void PluginInstance::UnscheduleTimer(uint32 timer_id) {
  TimerMap::iterator it = timers_.find(timer_id);
  if (it != timers_.end())
    timers_.erase(it);
}
