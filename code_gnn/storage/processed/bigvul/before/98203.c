WebSocketExperimentRunner::~WebSocketExperimentRunner() {
  DCHECK(!task_.get());
  WebSocketExperimentTask::ReleaseHistogram();
}
