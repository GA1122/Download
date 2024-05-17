RTCPeerConnectionHandler::~RTCPeerConnectionHandler() {
  DCHECK(task_runner_->RunsTasksInCurrentSequence());

  Stop();

  GetPeerConnectionHandlers()->erase(this);
  if (peer_connection_tracker_)
    peer_connection_tracker_->UnregisterPeerConnection(this);

  UMA_HISTOGRAM_COUNTS_10000(
      "WebRTC.NumDataChannelsPerPeerConnection", num_data_channels_created_);
}
