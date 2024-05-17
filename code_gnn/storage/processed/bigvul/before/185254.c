 bool WorkerProcessLauncherTest::LaunchProcess(
     IPC::Listener* delegate,
     ScopedHandle* process_exit_event_out) {
   process_exit_event_.Set(CreateEvent(NULL, TRUE, FALSE, NULL));
   if (!process_exit_event_.IsValid())
      return false;
  
    channel_name_ = GenerateIpcChannelName(this);
  if (!CreateIpcChannel(channel_name_, kIpcSecurityDescriptor, task_runner_,
                        delegate, &channel_server_)) {
//   ScopedHandle pipe;
//   if (!CreateIpcChannel(channel_name_, kIpcSecurityDescriptor, &pipe)) {
      return false;
    }
  
  exit_code_ = STILL_ACTIVE;
//    
//   channel_server_.reset(new IPC::ChannelProxy(
//       IPC::ChannelHandle(pipe),
//       IPC::Channel::MODE_SERVER,
//       delegate,
//       task_runner_));
// 
    return DuplicateHandle(GetCurrentProcess(),
                           process_exit_event_,
                           GetCurrentProcess(),
                          process_exit_event_out->Receive(),
                          0,
                          FALSE,
                          DUPLICATE_SAME_ACCESS) != FALSE;
 }