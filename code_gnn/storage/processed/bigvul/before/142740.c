void HTMLMediaElement::AddTextTrack(WebInbandTextTrack* web_track) {
  InbandTextTrack* text_track = InbandTextTrack::Create(web_track);


  text_track->SetReadinessState(TextTrack::kLoaded);

  ScheduleTextTrackResourceLoad();

  textTracks()->Append(text_track);
}
