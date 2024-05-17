void RendererSchedulerImpl::UpdatePolicyLocked(UpdateType update_type) {
  helper_.CheckOnValidThread();
  any_thread_lock_.AssertAcquired();
  if (helper_.IsShutdown())
    return;

  base::TimeTicks now = helper_.NowTicks();
  policy_may_need_update_.SetWhileLocked(false);

  base::TimeDelta expected_use_case_duration;
  UseCase use_case = ComputeCurrentUseCase(now, &expected_use_case_duration);
  main_thread_only().current_use_case = use_case;

  base::TimeDelta touchstart_expected_flag_valid_for_duration;
  bool touchstart_expected_soon = false;
  if (main_thread_only().has_visible_render_widget_with_touch_handler) {
    touchstart_expected_soon = any_thread().user_model.IsGestureExpectedSoon(
        now, &touchstart_expected_flag_valid_for_duration);
  }
  main_thread_only().touchstart_expected_soon = touchstart_expected_soon;

  base::TimeDelta longest_jank_free_task_duration =
      EstimateLongestJankFreeTaskDuration();
  main_thread_only().longest_jank_free_task_duration =
      longest_jank_free_task_duration;

  main_thread_only().loading_task_estimated_cost =
      main_thread_only().loading_task_cost_estimator.expected_task_duration();
  bool loading_tasks_seem_expensive =
      main_thread_only().loading_task_estimated_cost >
      longest_jank_free_task_duration;

  main_thread_only().timer_task_estimated_cost =
      main_thread_only().timer_task_cost_estimator.expected_task_duration();
  bool timer_tasks_seem_expensive =
      main_thread_only().timer_task_estimated_cost >
      longest_jank_free_task_duration;

  main_thread_only().timer_tasks_seem_expensive = timer_tasks_seem_expensive;
  main_thread_only().loading_tasks_seem_expensive =
      loading_tasks_seem_expensive;

  base::TimeDelta new_policy_duration = expected_use_case_duration;
  if (new_policy_duration.is_zero() ||
      (touchstart_expected_flag_valid_for_duration > base::TimeDelta() &&
       new_policy_duration > touchstart_expected_flag_valid_for_duration)) {
    new_policy_duration = touchstart_expected_flag_valid_for_duration;
  }

  if (main_thread_only().last_audio_state_change &&
      !main_thread_only().is_audio_playing) {
    UpdatePolicyDuration(now,
                         main_thread_only().last_audio_state_change.value() +
                             kThrottlingDelayAfterAudioIsPlayed,
                         &new_policy_duration);
  }

  bool previously_stopped_when_backgrounded =
      main_thread_only().stopped_when_backgrounded;
  bool newly_stopped = false;
  if (main_thread_only().renderer_backgrounded &&
      main_thread_only().stopping_when_backgrounded_enabled) {
    base::TimeTicks stop_at = main_thread_only().background_status_changed_at +
                              delay_for_background_tab_stopping_;

    newly_stopped = !main_thread_only().stopped_when_backgrounded;
    main_thread_only().stopped_when_backgrounded = now >= stop_at;
    newly_stopped &= main_thread_only().stopped_when_backgrounded;

    if (!main_thread_only().stopped_when_backgrounded)
      UpdatePolicyDuration(now, stop_at, &new_policy_duration);
  } else {
    main_thread_only().stopped_when_backgrounded = false;
  }

  if (new_policy_duration > base::TimeDelta()) {
    main_thread_only().current_policy_expiration_time =
        now + new_policy_duration;
    delayed_update_policy_runner_.SetDeadline(FROM_HERE, new_policy_duration,
                                              now);
  } else {
    main_thread_only().current_policy_expiration_time = base::TimeTicks();
  }

  bool main_thread_compositing_is_fast =
      main_thread_only().idle_time_estimator.GetExpectedIdleDuration(
          main_thread_only().compositor_frame_interval) >
      main_thread_only().compositor_frame_interval *
          kFastCompositingIdleTimeThreshold;

  Policy new_policy;
  ExpensiveTaskPolicy expensive_task_policy = ExpensiveTaskPolicy::kRun;
  new_policy.rail_mode() = v8::PERFORMANCE_ANIMATION;

  switch (use_case) {
    case UseCase::kCompositorGesture:
      if (touchstart_expected_soon) {
        new_policy.rail_mode() = v8::PERFORMANCE_RESPONSE;
        expensive_task_policy = ExpensiveTaskPolicy::kBlock;
        new_policy.compositor_queue_policy().priority =
            TaskQueue::kHighPriority;
      } else {
        new_policy.compositor_queue_policy().priority = TaskQueue::kLowPriority;
      }
      break;

    case UseCase::kSynchronizedGesture:
      new_policy.compositor_queue_policy().priority =
          main_thread_compositing_is_fast ? TaskQueue::kHighPriority
                                          : TaskQueue::kNormalPriority;
      if (touchstart_expected_soon) {
        new_policy.rail_mode() = v8::PERFORMANCE_RESPONSE;
        expensive_task_policy = ExpensiveTaskPolicy::kBlock;
      } else {
        expensive_task_policy = ExpensiveTaskPolicy::kThrottle;
      }
      break;

    case UseCase::kMainThreadCustomInputHandling:
      new_policy.compositor_queue_policy().priority =
          main_thread_compositing_is_fast ? TaskQueue::kHighPriority
                                          : TaskQueue::kNormalPriority;
      break;

    case UseCase::kMainThreadGesture:
      new_policy.compositor_queue_policy().priority = TaskQueue::kHighPriority;
      if (touchstart_expected_soon) {
        new_policy.rail_mode() = v8::PERFORMANCE_RESPONSE;
        expensive_task_policy = ExpensiveTaskPolicy::kBlock;
      } else {
        expensive_task_policy = ExpensiveTaskPolicy::kThrottle;
      }
      break;

    case UseCase::kTouchstart:
      new_policy.rail_mode() = v8::PERFORMANCE_RESPONSE;
      new_policy.compositor_queue_policy().priority = TaskQueue::kHighPriority;
      new_policy.loading_queue_policy().is_blocked = true;
      new_policy.timer_queue_policy().is_blocked = true;
      expensive_task_policy = ExpensiveTaskPolicy::kBlock;
      break;

    case UseCase::kNone:
      if (touchstart_expected_soon &&
          any_thread().last_gesture_was_compositor_driven) {
        new_policy.rail_mode() = v8::PERFORMANCE_RESPONSE;
        expensive_task_policy = ExpensiveTaskPolicy::kBlock;
      }
      break;

    case UseCase::kLoading:
      new_policy.rail_mode() = v8::PERFORMANCE_LOAD;
      break;

    default:
      NOTREACHED();
  }

  if (main_thread_only().renderer_hidden)
    new_policy.rail_mode() = v8::PERFORMANCE_IDLE;

  if (expensive_task_policy == ExpensiveTaskPolicy::kBlock &&
      (!main_thread_only().have_seen_a_begin_main_frame ||
       main_thread_only().navigation_task_expected_count > 0)) {
    expensive_task_policy = ExpensiveTaskPolicy::kRun;
  }

  switch (expensive_task_policy) {
    case ExpensiveTaskPolicy::kRun:
      break;

    case ExpensiveTaskPolicy::kBlock:
      if (loading_tasks_seem_expensive)
        new_policy.loading_queue_policy().is_blocked = true;
      if (timer_tasks_seem_expensive)
        new_policy.timer_queue_policy().is_blocked = true;
      break;

    case ExpensiveTaskPolicy::kThrottle:
      if (loading_tasks_seem_expensive) {
        new_policy.loading_queue_policy().is_throttled = true;
      }
      if (timer_tasks_seem_expensive) {
        new_policy.timer_queue_policy().is_throttled = true;
      }
      break;
  }
  main_thread_only().expensive_task_policy = expensive_task_policy;

  if (main_thread_only().stopped_when_backgrounded) {
    new_policy.timer_queue_policy().is_stopped = true;
    if (StopLoadingInBackgroundEnabled())
      new_policy.loading_queue_policy().is_stopped = true;
  }
  if (main_thread_only().renderer_pause_count != 0) {
    new_policy.loading_queue_policy().is_paused = true;
    new_policy.timer_queue_policy().is_paused = true;
  }
  if (main_thread_only().pause_timers_for_webview) {
    new_policy.timer_queue_policy().is_paused = true;
  }

  if (main_thread_only().renderer_backgrounded &&
      RuntimeEnabledFeatures::TimerThrottlingForBackgroundTabsEnabled()) {
    new_policy.timer_queue_policy().is_throttled = true;
  }

  if (main_thread_only().use_virtual_time) {
    new_policy.compositor_queue_policy().use_virtual_time = true;
    new_policy.default_queue_policy().use_virtual_time = true;
    new_policy.loading_queue_policy().use_virtual_time = true;
    new_policy.timer_queue_policy().use_virtual_time = true;
  }

  new_policy.should_disable_throttling() =
      ShouldDisableThrottlingBecauseOfAudio(now) ||
      main_thread_only().use_virtual_time;

  CreateTraceEventObjectSnapshotLocked();

  if (update_type == UpdateType::kMayEarlyOutIfPolicyUnchanged &&
      new_policy == main_thread_only().current_policy) {
    return;
  }

  for (const auto& pair : task_runners_) {
    MainThreadTaskQueue::QueueClass queue_class = pair.first->queue_class();

    ApplyTaskQueuePolicy(
        pair.first.get(), pair.second.get(),
        main_thread_only().current_policy.GetQueuePolicy(queue_class),
        new_policy.GetQueuePolicy(queue_class));
  }

  main_thread_only().rail_mode_for_tracing = new_policy.rail_mode();
  if (main_thread_only().rail_mode_observer &&
      new_policy.rail_mode() != main_thread_only().current_policy.rail_mode()) {
    main_thread_only().rail_mode_observer->OnRAILModeChanged(
        new_policy.rail_mode());
  }

  if (main_thread_only().stopping_when_backgrounded_enabled) {
    if (main_thread_only().stopped_when_backgrounded !=
        previously_stopped_when_backgrounded) {
      SetStoppedInBackground(main_thread_only().stopped_when_backgrounded);
      RendererMetricsHelper::RecordBackgroundedTransition(
          main_thread_only().stopped_when_backgrounded
              ? BackgroundedRendererTransition::kStoppedAfterDelay
              : BackgroundedRendererTransition::kResumed);
    }
  }

  if (new_policy.should_disable_throttling() !=
      main_thread_only().current_policy.should_disable_throttling()) {
    if (new_policy.should_disable_throttling()) {
      task_queue_throttler()->DisableThrottling();
    } else {
      task_queue_throttler()->EnableThrottling();
    }
  }

  DCHECK(compositor_task_queue_->IsQueueEnabled());
  main_thread_only().current_policy = new_policy;

  if (newly_stopped)
    Platform::Current()->RequestPurgeMemory();
}
