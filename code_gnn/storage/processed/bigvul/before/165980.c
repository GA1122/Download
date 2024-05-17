void RTCPeerConnectionHandler::GetStats(
    webrtc::StatsObserver* observer,
    webrtc::PeerConnectionInterface::StatsOutputLevel level,
    rtc::scoped_refptr<webrtc::MediaStreamTrackInterface> selector) {
  DCHECK(task_runner_->RunsTasksInCurrentSequence());
  signaling_thread()->PostTask(
      FROM_HERE,
      base::BindOnce(&GetStatsOnSignalingThread, native_peer_connection_, level,
                     base::WrapRefCounted(observer), std::move(selector)));
}
