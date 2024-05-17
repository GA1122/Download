void WorkerProcessLauncher::Send(IPC::Message* message) {
  core_->Send(message);
}
