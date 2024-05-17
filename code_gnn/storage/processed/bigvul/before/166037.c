  StatsResponse(const scoped_refptr<LocalRTCStatsRequest>& request,
                scoped_refptr<base::SingleThreadTaskRunner> task_runner)
      : request_(request.get()), main_thread_(task_runner) {
    TRACE_EVENT_ASYNC_BEGIN0("webrtc", "getStats_Native", this);
    DETACH_FROM_THREAD(signaling_thread_checker_);
  }
