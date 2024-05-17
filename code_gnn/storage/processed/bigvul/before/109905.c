bool GpuProcessHost::Init() {
  init_start_time_ = base::TimeTicks::Now();

  TRACE_EVENT_INSTANT0("gpu", "LaunchGpuProcess");

  std::string channel_id = process_->GetHost()->CreateChannel();
  if (channel_id.empty())
    return false;

  if (in_process_) {
    CommandLine::ForCurrentProcess()->AppendSwitch(
        switches::kDisableGpuWatchdog);

    in_process_gpu_thread_.reset(new GpuMainThread(channel_id));

    base::Thread::Options options;
#if defined(OS_WIN)
  options.message_loop_type = MessageLoop::TYPE_UI;
#else
  options.message_loop_type = MessageLoop::TYPE_IO;
#endif
    in_process_gpu_thread_->StartWithOptions(options);

    OnProcessLaunched();   
  } else if (!LaunchGpuProcess(channel_id)) {
    return false;
  }

  return Send(new GpuMsg_Initialize());
}
