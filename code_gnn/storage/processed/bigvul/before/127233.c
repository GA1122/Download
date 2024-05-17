WorkerProcessLauncher::~WorkerProcessLauncher() {
  core_->Stop();
  core_ = NULL;
}
