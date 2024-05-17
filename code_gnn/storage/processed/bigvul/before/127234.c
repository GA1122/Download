 void WorkerProcessLauncherTest::ConnectChannel() {
   channel_client_.reset(new IPC::ChannelProxy(
       IPC::ChannelHandle(channel_name_),
      IPC::Channel::MODE_CLIENT,
      this,
      task_runner_));
}
