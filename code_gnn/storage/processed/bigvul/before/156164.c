RendererSchedulerImpl::AsValueLocked(base::TimeTicks optional_now) const {
  helper_.CheckOnValidThread();
  any_thread_lock_.AssertAcquired();

  if (optional_now.is_null())
    optional_now = helper_.NowTicks();
  std::unique_ptr<base::trace_event::TracedValue> state(
      new base::trace_event::TracedValue());
  state->SetBoolean(
      "has_visible_render_widget_with_touch_handler",
      main_thread_only().has_visible_render_widget_with_touch_handler);
  state->SetString("current_use_case",
                   UseCaseToString(main_thread_only().current_use_case));
  state->SetBoolean("loading_tasks_seem_expensive",
                    main_thread_only().loading_tasks_seem_expensive);
  state->SetBoolean("timer_tasks_seem_expensive",
                    main_thread_only().timer_tasks_seem_expensive);
  state->SetBoolean("begin_frame_not_expected_soon",
                    main_thread_only().begin_frame_not_expected_soon);
  state->SetBoolean(
      "compositor_will_send_main_frame_not_expected",
      main_thread_only().compositor_will_send_main_frame_not_expected);
  state->SetBoolean("touchstart_expected_soon",
                    main_thread_only().touchstart_expected_soon);
  state->SetString("idle_period_state",
                   IdleHelper::IdlePeriodStateToString(
                       idle_helper_.SchedulerIdlePeriodState()));
  state->SetBoolean("renderer_hidden", main_thread_only().renderer_hidden);
  state->SetBoolean("have_seen_a_begin_main_frame",
                    main_thread_only().have_seen_a_begin_main_frame);
  state->SetBoolean("waiting_for_meaningful_paint",
                    any_thread().waiting_for_meaningful_paint);
  state->SetBoolean("have_seen_input_since_navigation",
                    any_thread().have_seen_input_since_navigation);
  state->SetBoolean(
      "have_reported_blocking_intervention_in_current_policy",
      main_thread_only().have_reported_blocking_intervention_in_current_policy);
  state->SetBoolean(
      "have_reported_blocking_intervention_since_navigation",
      main_thread_only().have_reported_blocking_intervention_since_navigation);
  state->SetBoolean("renderer_backgrounded",
                    main_thread_only().renderer_backgrounded);
  state->SetBoolean("stopped_when_backgrounded",
                    main_thread_only().stopped_when_backgrounded);
  state->SetDouble("now", (optional_now - base::TimeTicks()).InMillisecondsF());
  state->SetDouble(
      "fling_compositor_escalation_deadline",
      (any_thread().fling_compositor_escalation_deadline - base::TimeTicks())
          .InMillisecondsF());
  state->SetInteger("navigation_task_expected_count",
                    main_thread_only().navigation_task_expected_count);
  state->SetDouble("last_idle_period_end_time",
                   (any_thread().last_idle_period_end_time - base::TimeTicks())
                       .InMillisecondsF());
  state->SetBoolean("awaiting_touch_start_response",
                    any_thread().awaiting_touch_start_response);
  state->SetBoolean("begin_main_frame_on_critical_path",
                    any_thread().begin_main_frame_on_critical_path);
  state->SetBoolean("last_gesture_was_compositor_driven",
                    any_thread().last_gesture_was_compositor_driven);
  state->SetBoolean("default_gesture_prevented",
                    any_thread().default_gesture_prevented);
  state->SetDouble("expected_loading_task_duration",
                   main_thread_only()
                       .loading_task_cost_estimator.expected_task_duration()
                       .InMillisecondsF());
  state->SetDouble("expected_timer_task_duration",
                   main_thread_only()
                       .timer_task_cost_estimator.expected_task_duration()
                       .InMillisecondsF());
  state->SetBoolean("is_audio_playing", main_thread_only().is_audio_playing);
  state->SetBoolean("virtual_time_stopped",
                    main_thread_only().virtual_time_stopped);
  state->SetDouble("virtual_time_pause_count",
                   main_thread_only().virtual_time_pause_count);
  state->SetString(
      "virtual_time_policy",
      VirtualTimePolicyToString(main_thread_only().virtual_time_policy));
  state->SetBoolean("virtual_time", main_thread_only().use_virtual_time);

  state->BeginDictionary("web_view_schedulers");
  for (WebViewSchedulerImpl* web_view_scheduler :
       main_thread_only().web_view_schedulers) {
    state->BeginDictionaryWithCopiedName(PointerToString(web_view_scheduler));
    web_view_scheduler->AsValueInto(state.get());
    state->EndDictionary();
  }
  state->EndDictionary();

  state->BeginDictionary("policy");
  main_thread_only().current_policy.AsValueInto(state.get());
  state->EndDictionary();

  state->SetDouble(
      "longest_jank_free_task_duration",
      main_thread_only().longest_jank_free_task_duration->InMillisecondsF());
  state->SetDouble(
      "compositor_frame_interval",
      main_thread_only().compositor_frame_interval.InMillisecondsF());
  state->SetDouble(
      "estimated_next_frame_begin",
      (main_thread_only().estimated_next_frame_begin - base::TimeTicks())
          .InMillisecondsF());
  state->SetBoolean("in_idle_period", any_thread().in_idle_period);

  state->SetString(
      "expensive_task_policy",
      ExpensiveTaskPolicyToString(main_thread_only().expensive_task_policy));

  any_thread().user_model.AsValueInto(state.get());
  render_widget_scheduler_signals_.AsValueInto(state.get());

  state->BeginDictionary("task_queue_throttler");
  task_queue_throttler_->AsValueInto(state.get(), optional_now);
  state->EndDictionary();

  return std::move(state);
}
