ThreadWatcher::ThreadWatcher(const WatchingParams& params)
    : thread_id_(params.thread_id),
      thread_name_(params.thread_name),
      watched_runner_(
          BrowserThread::GetMessageLoopProxyForThread(params.thread_id)),
      sleep_time_(params.sleep_time),
      unresponsive_time_(params.unresponsive_time),
      ping_time_(base::TimeTicks::Now()),
      pong_time_(ping_time_),
      ping_sequence_number_(0),
      active_(false),
      ping_count_(params.unresponsive_threshold),
      response_time_histogram_(nullptr),
      unresponsive_time_histogram_(nullptr),
      unresponsive_count_(0),
      hung_processing_complete_(false),
      unresponsive_threshold_(params.unresponsive_threshold),
      crash_on_hang_(params.crash_on_hang),
      live_threads_threshold_(params.live_threads_threshold),
      weak_ptr_factory_(this) {
  DCHECK(WatchDogThread::CurrentlyOnWatchDogThread());
  Initialize();
}
