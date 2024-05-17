void WebMediaPlayerMS::TrackRemoved(const blink::WebMediaStreamTrack& track) {
  Reload();
}
