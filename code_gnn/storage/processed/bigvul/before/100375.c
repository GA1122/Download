bool BrowserRenderProcessHost::Init(bool is_extensions_process) {
  if (channel_.get())
    return true;

  base::Thread* io_thread = g_browser_process->io_thread();

  audio_renderer_host_ =
      new AudioRendererHost(io_thread->message_loop());

  scoped_refptr<ResourceMessageFilter> resource_message_filter =
      new ResourceMessageFilter(g_browser_process->resource_dispatcher_host(),
                                id(),
                                audio_renderer_host_.get(),
                                PluginService::GetInstance(),
                                g_browser_process->print_job_manager(),
                                profile(),
                                widget_helper_,
                                profile()->GetSpellChecker());

  FilePath renderer_path = ChildProcessHost::GetChildPath();
  if (renderer_path.empty())
    return false;

  const std::string channel_id =
      ChildProcessInfo::GenerateRandomChannelID(this);
  channel_.reset(
      new IPC::SyncChannel(channel_id, IPC::Channel::MODE_SERVER, this,
                           resource_message_filter,
                           io_thread->message_loop(), true,
                           g_browser_process->shutdown_event()));
  channel_->set_sync_messages_with_no_timeout_allowed(false);

  CommandLine cmd_line(renderer_path);
  cmd_line.AppendSwitchWithValue(switches::kProcessChannelID,
                                 ASCIIToWide(channel_id));
  if (is_extensions_process)
    cmd_line.AppendSwitch(switches::kEnableDatabases);
  bool has_cmd_prefix;
  AppendRendererCommandLine(&cmd_line, &has_cmd_prefix);

  if (run_renderer_in_process()) {
    in_process_renderer_.reset(new RendererMainThread(channel_id));

    base::Thread::Options options;
#if !defined(OS_LINUX)
    options.message_loop_type = MessageLoop::TYPE_UI;
#else
    options.message_loop_type = MessageLoop::TYPE_DEFAULT;
#endif
    in_process_renderer_->StartWithOptions(options);
  } else {
    base::TimeTicks begin_launch_time = base::TimeTicks::Now();

    base::ProcessHandle process = ExecuteRenderer(&cmd_line, has_cmd_prefix);
    if (!process) {
      channel_.reset();
      return false;
    }
    process_.set_handle(process);
    fast_shutdown_started_ = false;

    static bool done_first_launch = false;
    if (done_first_launch) {
      UMA_HISTOGRAM_TIMES("MPArch.RendererLaunchSubsequent",
                          base::TimeTicks::Now() - begin_launch_time);
    } else {
      UMA_HISTOGRAM_TIMES("MPArch.RendererLaunchFirst",
                          base::TimeTicks::Now() - begin_launch_time);
      done_first_launch = true;
    }
  }

  resource_message_filter->Init();

  SetBackgrounded(backgrounded_);

  InitVisitedLinks();
  InitUserScripts();
  InitExtensions();

  if (max_page_id_ != -1)
    channel_->Send(new ViewMsg_SetNextPageID(max_page_id_ + 1));

  return true;
}
