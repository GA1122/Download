void HTMLMediaElement::RemoveTextTrack(WebInbandTextTrack* web_track) {
  if (!text_tracks_)
    return;

  InbandTextTrack* text_track = ToInbandTextTrack(web_track->Client());
  if (!text_track)
    return;

  text_tracks_->Remove(text_track);
}
