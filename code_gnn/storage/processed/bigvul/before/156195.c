base::TimeDelta RendererSchedulerImpl::EstimateLongestJankFreeTaskDuration()
    const {
  switch (main_thread_only().current_use_case) {
    case UseCase::kTouchstart:
    case UseCase::kCompositorGesture:
    case UseCase::kLoading:
    case UseCase::kNone:
      return base::TimeDelta::FromMilliseconds(kRailsResponseTimeMillis);

    case UseCase::kMainThreadCustomInputHandling:
    case UseCase::kMainThreadGesture:
    case UseCase::kSynchronizedGesture:
      return main_thread_only().idle_time_estimator.GetExpectedIdleDuration(
          main_thread_only().compositor_frame_interval);

    default:
      NOTREACHED();
      return base::TimeDelta::FromMilliseconds(kRailsResponseTimeMillis);
  }
}
