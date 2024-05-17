bool BrowserRenderProcessHost::Init(bool is_accessibility_enabled) {
  if (channel_.get())
    return true;

  accessibility_enabled_ = is_accessibility_enabled;

  CommandLine::StringType renderer_prefix;
#if defined(OS_POSIX)
  const CommandLine& browser_command_line = *CommandLine::ForCurrentProcess();
  renderer_prefix =
      browser_command_line.GetSwitchValueNative(switches::kRendererCmdPrefix);
#endif   

  FilePath renderer_path =
      ChildProcessHost::GetChildPath(renderer_prefix.empty());
  if (renderer_path.empty())
    return false;

  const std::string channel_id =
      ChildProcessInfo::GenerateRandomChannelID(this);
  channel_.reset(new IPC::ChannelProxy(
      channel_id, IPC::Channel::MODE_SERVER, this,
      BrowserThread::GetMessageLoopProxyForThread(BrowserThread::IO)));

  content::GetContentClient()->browser()->BrowserRenderProcessHostCreated(this);

  CreateMessageFilters();

  if (run_renderer_in_process()) {
    in_process_renderer_.reset(new RendererMainThread(channel_id));

    base::Thread::Options options;
#if !defined(TOOLKIT_USES_GTK)
    options.message_loop_type = MessageLoop::TYPE_UI;
#else
    options.message_loop_type = MessageLoop::TYPE_DEFAULT;
#endif
    in_process_renderer_->StartWithOptions(options);

    OnProcessLaunched();   
  } else {
    CommandLine* cmd_line = new CommandLine(renderer_path);
    if (!renderer_prefix.empty())
      cmd_line->PrependWrapper(renderer_prefix);
    AppendRendererCommandLine(cmd_line);
    cmd_line->AppendSwitchASCII(switches::kProcessChannelID, channel_id);

    child_process_launcher_.reset(new ChildProcessLauncher(
#if defined(OS_WIN)
        FilePath(),
#elif defined(OS_POSIX)
        renderer_prefix.empty(),
        base::environment_vector(),
        channel_->GetClientFileDescriptor(),
#endif
        cmd_line,
        this));

    fast_shutdown_started_ = false;
  }

  is_initialized_ = true;
  return true;
}
