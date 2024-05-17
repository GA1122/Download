void HTMLMediaElement::DidAddTrackElement(HTMLTrackElement* track_element) {
  TextTrack* text_track = track_element->track();
  if (!text_track)
    return;

  textTracks()->Append(text_track);

  if (IsFinishedParsingChildren())
    ScheduleTextTrackResourceLoad();
}
