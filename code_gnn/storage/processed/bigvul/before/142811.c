bool HTMLMediaElement::HasSelectedVideoTrack() {
  return video_tracks_ && video_tracks_->selectedIndex() != -1;
}
