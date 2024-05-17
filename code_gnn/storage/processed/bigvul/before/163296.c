void BrowserMainLoop::ShutdownThreadsAndCleanUp() {
  if (!created_threads_) {
    return;
  }
  TRACE_EVENT0("shutdown", "BrowserMainLoop::ShutdownThreadsAndCleanUp");

  base::ThreadRestrictions::SetIOAllowed(true);
  BrowserThread::PostTask(
      BrowserThread::IO, FROM_HERE,
      base::BindOnce(
          base::IgnoreResult(&base::ThreadRestrictions::SetIOAllowed), true));

#if defined(OS_ANDROID)
  g_browser_main_loop_shutting_down = true;
#endif

  if (RenderProcessHost::run_renderer_in_process())
    RenderProcessHostImpl::ShutDownInProcessRenderer();

  if (parts_) {
    TRACE_EVENT0("shutdown",
                 "BrowserMainLoop::Subsystem:PostMainMessageLoopRun");
    parts_->PostMainMessageLoopRun();
  }

  system_stats_monitor_.reset();

  if (resource_dispatcher_host_) {
    TRACE_EVENT0("shutdown",
                 "BrowserMainLoop::Subsystem:ResourceDispatcherHost");
    resource_dispatcher_host_->Shutdown();
  }
  if (midi_service_) {
    TRACE_EVENT0("shutdown", "BrowserMainLoop::Subsystem:MidiService");
    midi_service_->Shutdown();
  }

  TRACE_EVENT0("shutdown",
               "BrowserMainLoop::Subsystem:SpeechRecognitionManager");
  io_thread_->task_runner()->DeleteSoon(FROM_HERE,
                                        speech_recognition_manager_.release());

  memory_pressure_monitor_.reset();

#if defined(OS_MACOSX)
  BrowserCompositorMac::DisableRecyclingForShutdown();
#endif

#if defined(USE_AURA) || defined(OS_MACOSX)
  {
    TRACE_EVENT0("shutdown",
                 "BrowserMainLoop::Subsystem:ImageTransportFactory");
    ImageTransportFactory::Terminate();
  }
#endif

#if !defined(OS_ANDROID)
  host_frame_sink_manager_.reset();
  frame_sink_manager_impl_.reset();
#endif
  compositing_mode_reporter_impl_.reset();

#if defined(OS_WIN)
  system_message_window_.reset();
#elif defined(OS_MACOSX)
  device_monitor_mac_.reset();
#endif

  if (BrowserGpuChannelHostFactory::instance()) {
    BrowserGpuChannelHostFactory::instance()->CloseChannel();
  }

  service_manager_context_.reset();
  mojo_ipc_support_.reset();

  if (save_file_manager_)
    save_file_manager_->Shutdown();

  {
    base::ThreadRestrictions::ScopedAllowWait allow_wait_for_join;

    for (size_t thread_id = BrowserThread::ID_COUNT - 1;
         thread_id >= (BrowserThread::UI + 1); --thread_id) {
      switch (thread_id) {
        case BrowserThread::DB: {
          TRACE_EVENT0("shutdown", "BrowserMainLoop::Subsystem:DBThread");
          ResetThread_DB();
          break;
        }
        case BrowserThread::FILE: {
          TRACE_EVENT0("shutdown", "BrowserMainLoop::Subsystem:FileThread");
          ResetThread_FILE();
          break;
        }
        case BrowserThread::FILE_USER_BLOCKING: {
          TRACE_EVENT0("shutdown",
                       "BrowserMainLoop::Subsystem:FileUserBlockingThread");
          ResetThread_FILE_USER_BLOCKING();
          break;
        }
        case BrowserThread::PROCESS_LAUNCHER: {
          TRACE_EVENT0("shutdown", "BrowserMainLoop::Subsystem:LauncherThread");
#if defined(OS_ANDROID)
          ResetThread_PROCESS_LAUNCHER(std::move(process_launcher_thread_));
#else    
          ResetThread_PROCESS_LAUNCHER();
#endif   
          break;
        }
        case BrowserThread::CACHE: {
          TRACE_EVENT0("shutdown", "BrowserMainLoop::Subsystem:CacheThread");
#if defined(OS_WIN)
          ResetThread_CACHE(std::move(cache_thread_));
#else    
          ResetThread_CACHE();
#endif   
          break;
        }
        case BrowserThread::IO: {
          TRACE_EVENT0("shutdown", "BrowserMainLoop::Subsystem:IOThread");
          ResetThread_IO(std::move(io_thread_));
          break;
        }
        case BrowserThread::UI:
        case BrowserThread::ID_COUNT:
          NOTREACHED();
          break;
      }
    }

    {
      TRACE_EVENT0("shutdown", "BrowserMainLoop::Subsystem:TaskScheduler");
      base::TaskScheduler::GetInstance()->Shutdown();
    }
  }

  {
    TRACE_EVENT0("shutdown", "BrowserMainLoop::Subsystem:GPUChannelFactory");
    if (BrowserGpuChannelHostFactory::instance()) {
      BrowserGpuChannelHostFactory::Terminate();
    }
  }

  {
    TRACE_EVENT0("shutdown", "BrowserMainLoop::Subsystem:GamepadService");
    device::GamepadService::GetInstance()->Terminate();
  }
  {
    TRACE_EVENT0("shutdown", "BrowserMainLoop::Subsystem:DeleteDataSources");
    URLDataManager::DeleteDataSources();
  }
  {
    TRACE_EVENT0("shutdown", "BrowserMainLoop::Subsystem:AudioMan");
    if (audio_manager_ && !audio_manager_->Shutdown()) {
      ignore_result(audio_manager_.release());
      ignore_result(user_input_monitor_.release());
    }
  }

  if (parts_) {
    TRACE_EVENT0("shutdown", "BrowserMainLoop::Subsystem:PostDestroyThreads");
    parts_->PostDestroyThreads();
  }
}
