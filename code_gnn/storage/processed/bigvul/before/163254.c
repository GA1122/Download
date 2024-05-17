int BrowserMainLoop::CreateThreads() {
  TRACE_EVENT0("startup,rail", "BrowserMainLoop::CreateThreads");

  {
    auto task_scheduler_init_params =
        GetContentClient()->browser()->GetTaskSchedulerInitParams();
    if (!task_scheduler_init_params)
      task_scheduler_init_params = GetDefaultTaskSchedulerInitParams();
    DCHECK(task_scheduler_init_params);

    if (base::CommandLine::ForCurrentProcess()->HasSwitch(
            switches::kSingleProcess)) {
      const base::SchedulerWorkerPoolParams&
          current_foreground_worker_pool_params(
              task_scheduler_init_params->foreground_worker_pool_params);
      task_scheduler_init_params->foreground_worker_pool_params =
          base::SchedulerWorkerPoolParams(
              std::max(GetMinThreadsInRendererTaskSchedulerForegroundPool(),
                       current_foreground_worker_pool_params.max_threads()),
              current_foreground_worker_pool_params.suggested_reclaim_time(),
              current_foreground_worker_pool_params.backward_compatibility());
    }

    base::TaskScheduler::GetInstance()->Start(
        *task_scheduler_init_params.get());
  }

  base::SequencedWorkerPool::EnableWithRedirectionToTaskSchedulerForProcess();

  base::Thread::Options io_message_loop_options;
  io_message_loop_options.message_loop_type = base::MessageLoop::TYPE_IO;

  for (size_t thread_id = BrowserThread::UI + 1;
       thread_id < BrowserThread::ID_COUNT;
       ++thread_id) {
    std::unique_ptr<BrowserProcessSubThread>* thread_to_start = nullptr;
    base::Thread::Options options;
    base::MessageLoop* message_loop = nullptr;

    base::TaskTraits non_ui_non_io_task_runner_traits;

    constexpr base::TaskTraits kUserVisibleTraits = {
        base::MayBlock(), base::WithBaseSyncPrimitives(),
        base::TaskPriority::USER_VISIBLE,
        base::TaskShutdownBehavior::BLOCK_SHUTDOWN};
    constexpr base::TaskTraits kUserBlockingTraits = {
        base::MayBlock(), base::WithBaseSyncPrimitives(),
        base::TaskPriority::USER_BLOCKING,
        base::TaskShutdownBehavior::BLOCK_SHUTDOWN};

    switch (thread_id) {
      case BrowserThread::DB:
        TRACE_EVENT_BEGIN1("startup",
            "BrowserMainLoop::CreateThreads:start",
            "Thread", "BrowserThread::DB");
        non_ui_non_io_task_runner_traits = kUserVisibleTraits;
        break;
      case BrowserThread::FILE_USER_BLOCKING:
        TRACE_EVENT_BEGIN1("startup",
            "BrowserMainLoop::CreateThreads:start",
            "Thread", "BrowserThread::FILE_USER_BLOCKING");
        non_ui_non_io_task_runner_traits = kUserBlockingTraits;
        break;
      case BrowserThread::FILE:
        TRACE_EVENT_BEGIN1("startup",
            "BrowserMainLoop::CreateThreads:start",
            "Thread", "BrowserThread::FILE");
        non_ui_non_io_task_runner_traits = kUserVisibleTraits;
        break;
      case BrowserThread::PROCESS_LAUNCHER:
        TRACE_EVENT_BEGIN1("startup",
            "BrowserMainLoop::CreateThreads:start",
            "Thread", "BrowserThread::PROCESS_LAUNCHER");
#if defined(OS_ANDROID)
        message_loop = android::LauncherThread::GetMessageLoop();
        DCHECK(message_loop);
        thread_to_start = &process_launcher_thread_;
#else    
        non_ui_non_io_task_runner_traits = kUserBlockingTraits;
#endif   
        break;
      case BrowserThread::CACHE:
        TRACE_EVENT_BEGIN1("startup",
            "BrowserMainLoop::CreateThreads:start",
            "Thread", "BrowserThread::CACHE");
#if defined(OS_WIN)
        thread_to_start = &cache_thread_;
        options = io_message_loop_options;
        options.timer_slack = base::TIMER_SLACK_MAXIMUM;
#else   
        non_ui_non_io_task_runner_traits = kUserBlockingTraits;
#endif   
        break;
      case BrowserThread::IO:
        TRACE_EVENT_BEGIN1("startup",
            "BrowserMainLoop::CreateThreads:start",
            "Thread", "BrowserThread::IO");
        thread_to_start = &io_thread_;
        options = io_message_loop_options;
#if defined(OS_ANDROID) || defined(OS_CHROMEOS)
        options.priority = base::ThreadPriority::DISPLAY;
#endif
        break;
      case BrowserThread::UI:         
      case BrowserThread::ID_COUNT:   
        NOTREACHED();
        break;
    }

    BrowserThread::ID id = static_cast<BrowserThread::ID>(thread_id);

    if (thread_to_start) {
      (*thread_to_start)
          .reset(message_loop ? new BrowserProcessSubThread(id, message_loop)
                              : new BrowserProcessSubThread(id));
      if (!message_loop && !(*thread_to_start)->StartWithOptions(options))
        LOG(FATAL) << "Failed to start the browser thread: id == " << id;
    } else {
      scoped_refptr<base::SingleThreadTaskRunner> redirection_task_runner;
#if defined(OS_WIN)
      redirection_task_runner =
          (thread_id == BrowserThread::FILE)
              ? base::CreateCOMSTATaskRunnerWithTraits(
                    non_ui_non_io_task_runner_traits,
                    base::SingleThreadTaskRunnerThreadMode::DEDICATED)
              : base::CreateSingleThreadTaskRunnerWithTraits(
                    non_ui_non_io_task_runner_traits,
                    base::SingleThreadTaskRunnerThreadMode::DEDICATED);
#else    
      redirection_task_runner = base::CreateSingleThreadTaskRunnerWithTraits(
          non_ui_non_io_task_runner_traits,
          base::SingleThreadTaskRunnerThreadMode::DEDICATED);
#endif   
      DCHECK(redirection_task_runner);
      BrowserThreadImpl::RedirectThreadIDToTaskRunner(
          id, std::move(redirection_task_runner));
    }

    TRACE_EVENT_END0("startup", "BrowserMainLoop::CreateThreads:start");
  }
  created_threads_ = true;
  return result_code_;
}
