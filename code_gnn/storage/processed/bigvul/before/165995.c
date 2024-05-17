MediaStreamTrackMetrics::Kind MediaStreamTrackMetricsKind(
    const blink::WebMediaStreamTrack& track) {
  return track.Source().GetType() == blink::WebMediaStreamSource::kTypeAudio
             ? MediaStreamTrackMetrics::Kind::kAudio
             : MediaStreamTrackMetrics::Kind::kVideo;
}
