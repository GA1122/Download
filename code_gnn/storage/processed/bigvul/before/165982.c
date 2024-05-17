void GetStatsOnSignalingThread(
    const scoped_refptr<webrtc::PeerConnectionInterface>& pc,
    webrtc::PeerConnectionInterface::StatsOutputLevel level,
    const scoped_refptr<webrtc::StatsObserver>& observer,
    rtc::scoped_refptr<webrtc::MediaStreamTrackInterface> selector) {
  TRACE_EVENT0("webrtc", "GetStatsOnSignalingThread");

  if (selector) {
    bool belongs_to_pc = false;
    for (const auto& sender : pc->GetSenders()) {
      if (sender->track() == selector) {
        belongs_to_pc = true;
        break;
      }
    }
    if (!belongs_to_pc) {
      for (const auto& receiver : pc->GetReceivers()) {
        if (receiver->track() == selector) {
          belongs_to_pc = true;
          break;
        }
      }
    }
    if (!belongs_to_pc) {
      DVLOG(1) << "GetStats: Track not found.";
      observer->OnComplete(StatsReports());
      return;
    }
  }

  if (!pc->GetStats(observer.get(), selector.get(), level)) {
    DVLOG(1) << "GetStats failed.";
    observer->OnComplete(StatsReports());
  }
}
