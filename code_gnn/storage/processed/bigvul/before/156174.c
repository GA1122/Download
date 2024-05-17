RendererSchedulerImpl::UseCase RendererSchedulerImpl::ComputeCurrentUseCase(
    base::TimeTicks now,
    base::TimeDelta* expected_use_case_duration) const {
  any_thread_lock_.AssertAcquired();
  if (any_thread().fling_compositor_escalation_deadline > now &&
      !any_thread().awaiting_touch_start_response) {
    *expected_use_case_duration =
        any_thread().fling_compositor_escalation_deadline - now;
    return UseCase::kCompositorGesture;
  }
  *expected_use_case_duration =
      any_thread().user_model.TimeLeftInUserGesture(now);
  if (*expected_use_case_duration > base::TimeDelta()) {
    if (any_thread().awaiting_touch_start_response) {
      return UseCase::kTouchstart;
    }

    if (any_thread().last_gesture_was_compositor_driven) {
      if (any_thread().begin_main_frame_on_critical_path) {
        return UseCase::kSynchronizedGesture;
      } else {
        return UseCase::kCompositorGesture;
      }
    }
    if (any_thread().default_gesture_prevented) {
      return UseCase::kMainThreadCustomInputHandling;
    } else {
      return UseCase::kMainThreadGesture;
    }
  }

  if (any_thread().waiting_for_meaningful_paint &&
      !any_thread().have_seen_input_since_navigation) {
    return UseCase::kLoading;
  }
  return UseCase::kNone;
}
