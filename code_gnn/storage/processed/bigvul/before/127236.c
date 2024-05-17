bool WorkerProcessLauncherTest::LaunchProcessAndConnect(
    IPC::Listener* delegate,
    ScopedHandle* process_exit_event_out) {
  if (!LaunchProcess(delegate, process_exit_event_out))
    return false;

  task_runner_->PostTask(
      FROM_HERE,
      base::Bind(&WorkerProcessLauncherTest::ConnectChannel,
                 base::Unretained(this)));
   return true;
 }
