void ThreadWatcher::GotNoResponse() {
  DCHECK(WatchDogThread::CurrentlyOnWatchDogThread());

  ++unresponsive_count_;
  if (!IsVeryUnresponsive())
    return;

  base::TimeDelta unresponse_time = base::TimeTicks::Now() - pong_time_;
  unresponsive_time_histogram_->AddTime(unresponse_time);

  if (hung_processing_complete_)
    return;

  uint32 responding_thread_count = 0;
  uint32 unresponding_thread_count = 0;
  ThreadWatcherList::GetStatusOfThreads(&responding_thread_count,
                                        &unresponding_thread_count);

  responsive_count_histogram_->Add(responding_thread_count);

  unresponsive_count_histogram_->Add(unresponding_thread_count);

  if (crash_on_hang_ &&
      responding_thread_count > 0 &&
      responding_thread_count <= live_threads_threshold_) {
    static bool crashed_once = false;
    if (!crashed_once) {
      crashed_once = true;
      metrics::CrashBecauseThreadWasUnresponsive(thread_id_);
    }
  }

  hung_processing_complete_ = true;
}
