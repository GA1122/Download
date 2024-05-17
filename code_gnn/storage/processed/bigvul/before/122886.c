bool RenderProcessHostImpl::Init() {
  if (channel_.get())
    return true;

  CommandLine::StringType renderer_prefix;
#if defined(OS_POSIX)
  const CommandLine& browser_command_line = *CommandLine::ForCurrentProcess();
  renderer_prefix =
      browser_command_line.GetSwitchValueNative(switches::kRendererCmdPrefix);
#endif   

#if defined(OS_LINUX)
  int flags = renderer_prefix.empty() ? ChildProcessHost::CHILD_ALLOW_SELF :
                                        ChildProcessHost::CHILD_NORMAL;
#else
  int flags = ChildProcessHost::CHILD_NORMAL;
#endif

  FilePath renderer_path = ChildProcessHost::GetChildPath(flags);
  if (renderer_path.empty())
    return false;

  const std::string channel_id =
      IPC::Channel::GenerateVerifiedChannelID(std::string());
  channel_.reset(
#if defined(OS_ANDROID)
      browser_command_line.HasSwitch(switches::kEnableWebViewSynchronousAPIs) ?
          new IPC::SyncChannel(
              channel_id, IPC::Channel::MODE_SERVER, this,
              BrowserThread::GetMessageLoopProxyForThread(BrowserThread::IO),
              true, &dummy_shutdown_event_) :
#endif
      new IPC::ChannelProxy(
          channel_id, IPC::Channel::MODE_SERVER, this,
          BrowserThread::GetMessageLoopProxyForThread(BrowserThread::IO)));

  GetContentClient()->browser()->RenderProcessHostCreated(this);

  CreateMessageFilters();

  if (run_renderer_in_process()) {
    in_process_renderer_.reset(new RendererMainThread(channel_id));

    base::Thread::Options options;
#if !defined(TOOLKIT_GTK) && !defined(OS_ANDROID)
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
        base::EnvironmentVector(),
        channel_->TakeClientFileDescriptor(),
#endif
        cmd_line,
        GetID(),
        this));

    fast_shutdown_started_ = false;
  }

  is_initialized_ = true;
  return true;
}
