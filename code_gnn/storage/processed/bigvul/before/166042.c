void LocalRTCStatsResponse::addStats(const blink::WebRTCLegacyStats& stats) {
  impl_.AddStats(stats);
}
