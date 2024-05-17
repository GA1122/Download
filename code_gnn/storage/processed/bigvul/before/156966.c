bool IsPlayableTrack(const blink::WebMediaStreamTrack& track) {
  return !track.IsNull() && !track.Source().IsNull() &&
         track.Source().GetReadyState() !=
             blink::WebMediaStreamSource::kReadyStateEnded;
}
