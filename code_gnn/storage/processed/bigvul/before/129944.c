void CaptureProfiles(const SamplingParams& params, TimeDelta profiler_wait_time,
                     CallStackProfiles* profiles) {
  profiles->clear();

  WithTargetThread([&params, profiles, profiler_wait_time](
      PlatformThreadId target_thread_id) {
    WaitableEvent sampling_thread_completed(true, false);
    const StackSamplingProfiler::CompletedCallback callback =
        Bind(&SaveProfilesAndSignalEvent, Unretained(profiles),
             Unretained(&sampling_thread_completed));
    StackSamplingProfiler profiler(target_thread_id, params, callback);
    profiler.Start();
    sampling_thread_completed.TimedWait(profiler_wait_time);
    profiler.Stop();
    sampling_thread_completed.Wait();
  });
}
