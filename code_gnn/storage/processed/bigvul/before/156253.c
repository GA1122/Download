RendererSchedulerImpl::RendererSchedulerImpl(
    std::unique_ptr<TaskQueueManager> task_queue_manager)
    : helper_(std::move(task_queue_manager), this),
      idle_helper_(
          &helper_,
          this,
          "RendererSchedulerIdlePeriod",
          base::TimeDelta(),
          helper_.NewTaskQueue(MainThreadTaskQueue::QueueCreationParams(
              MainThreadTaskQueue::QueueType::kIdle))),
      idle_canceled_delayed_task_sweeper_(&helper_,
                                          idle_helper_.IdleTaskRunner()),
      render_widget_scheduler_signals_(this),
      control_task_queue_(helper_.ControlMainThreadTaskQueue()),
      compositor_task_queue_(
          helper_.NewTaskQueue(MainThreadTaskQueue::QueueCreationParams(
                                   MainThreadTaskQueue::QueueType::kCompositor)
                                   .SetShouldMonitorQuiescence(true))),
      input_task_queue_(
          helper_.NewTaskQueue(MainThreadTaskQueue::QueueCreationParams(
                                   MainThreadTaskQueue::QueueType::kInput)
                                   .SetShouldMonitorQuiescence(true))),
      compositor_task_queue_enabled_voter_(
          compositor_task_queue_->CreateQueueEnabledVoter()),
      input_task_queue_enabled_voter_(
          input_task_queue_->CreateQueueEnabledVoter()),
      delayed_update_policy_runner_(
          base::Bind(&RendererSchedulerImpl::UpdatePolicy,
                     base::Unretained(this)),
          helper_.ControlMainThreadTaskQueue()),
      seqlock_queueing_time_estimator_(
          QueueingTimeEstimator(this, kQueueingTimeWindowDuration, 20)),
      main_thread_only_(this,
                        compositor_task_queue_,
                        helper_.GetClock(),
                        helper_.NowTicks()),
      any_thread_(this),
      policy_may_need_update_(&any_thread_lock_),
      weak_factory_(this) {
  task_queue_throttler_.reset(
      new TaskQueueThrottler(this, &tracing_controller_));
  update_policy_closure_ = base::Bind(&RendererSchedulerImpl::UpdatePolicy,
                                      weak_factory_.GetWeakPtr());
  end_renderer_hidden_idle_period_closure_.Reset(base::Bind(
      &RendererSchedulerImpl::EndIdlePeriod, weak_factory_.GetWeakPtr()));

  task_runners_.insert(
      std::make_pair(helper_.DefaultMainThreadTaskQueue(), nullptr));
  task_runners_.insert(
      std::make_pair(compositor_task_queue_,
                     compositor_task_queue_->CreateQueueEnabledVoter()));
  task_runners_.insert(std::make_pair(
      input_task_queue_, input_task_queue_->CreateQueueEnabledVoter()));

  default_timer_task_queue_ =
      NewTimerTaskQueue(MainThreadTaskQueue::QueueType::kDefaultTimer);
  v8_task_queue_ = NewTaskQueue(MainThreadTaskQueue::QueueCreationParams(
      MainThreadTaskQueue::QueueType::kV8));
  ipc_task_queue_ = NewTaskQueue(MainThreadTaskQueue::QueueCreationParams(
      MainThreadTaskQueue::QueueType::kIPC));

  TRACE_EVENT_OBJECT_CREATED_WITH_ID(
      TRACE_DISABLED_BY_DEFAULT("renderer.scheduler"), "RendererScheduler",
      this);

  helper_.SetObserver(this);

  if (base::ThreadTaskRunnerHandle::IsSet()) {
    base::trace_event::TraceLog::GetInstance()->AddAsyncEnabledStateObserver(
        weak_factory_.GetWeakPtr());
  }

  int32_t delay_for_background_tab_stopping_millis;
  if (!base::StringToInt(
          base::GetFieldTrialParamValue("BackgroundTabStopping",
                                        "DelayForBackgroundTabStoppingMills"),
          &delay_for_background_tab_stopping_millis)) {
    delay_for_background_tab_stopping_millis =
        kDelayForBackgroundTabStoppingMillis;
  }
  delay_for_background_tab_stopping_ = base::TimeDelta::FromMilliseconds(
      delay_for_background_tab_stopping_millis);

  internal::ProcessState::Get()->is_process_backgrounded =
      main_thread_only().renderer_backgrounded;
}
