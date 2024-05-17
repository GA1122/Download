void RenderThread::InformHostOfCacheStatsLater() {
  if (cache_stats_task_pending_)
    return;

  cache_stats_task_pending_ = true;
  MessageLoop::current()->PostDelayedTask(FROM_HERE,
      task_factory_->NewRunnableMethod(
          &RenderThread::InformHostOfCacheStats),
      kCacheStatsDelayMS);
}
