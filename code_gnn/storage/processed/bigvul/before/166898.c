Performance::Performance(
    TimeTicks time_origin,
    scoped_refptr<base::SingleThreadTaskRunner> task_runner)
    : frame_timing_buffer_size_(kDefaultFrameTimingBufferSize),
      resource_timing_buffer_size_(kDefaultResourceTimingBufferSize),
      user_timing_(nullptr),
      time_origin_(time_origin),
      observer_filter_options_(PerformanceEntry::kInvalid),
      deliver_observations_timer_(std::move(task_runner),
                                  this,
                                  &Performance::DeliverObservationsTimerFired) {
}
