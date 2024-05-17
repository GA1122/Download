RTCPeerConnectionHandler::signaling_thread() const {
  DCHECK(task_runner_->RunsTasksInCurrentSequence());
  return dependency_factory_->GetWebRtcSignalingThread();
}
