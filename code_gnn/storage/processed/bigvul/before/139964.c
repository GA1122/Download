void HTMLMediaElement::didAddTrackElement(HTMLTrackElement* trackElement) {
  TextTrack* textTrack = trackElement->track();
  if (!textTrack)
    return;

  textTracks()->append(textTrack);

  if (isFinishedParsingChildren())
    scheduleTextTrackResourceLoad();
}
