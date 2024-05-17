void WebMediaPlayerMS::TrackAdded(const blink::WebMediaStreamTrack& track) {
  Reload();
}
