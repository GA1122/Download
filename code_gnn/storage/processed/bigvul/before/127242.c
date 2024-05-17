void WorkerProcessLauncherTest::StopWorker() {
  launcher_.reset();
  DisconnectChannel();
  channel_name_.clear();
  channel_server_.reset();
  task_runner_ = NULL;
}
