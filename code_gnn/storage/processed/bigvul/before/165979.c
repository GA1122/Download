void RTCPeerConnectionHandler::GetStats(
    const blink::WebRTCStatsRequest& request) {
  DCHECK(task_runner_->RunsTasksInCurrentSequence());
  scoped_refptr<LocalRTCStatsRequest> inner_request(
      new rtc::RefCountedObject<LocalRTCStatsRequest>(request));
  getStats(inner_request);
}
