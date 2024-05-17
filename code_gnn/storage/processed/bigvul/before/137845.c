Element* MediaControlTextTrackListElement::createTextTrackListItem(
    TextTrack* track) {
  int trackIndex = track ? track->trackIndex() : trackIndexOffValue;
  HTMLLabelElement* trackItem = HTMLLabelElement::create(document());
  trackItem->setShadowPseudoId(
      AtomicString("-internal-media-controls-text-track-list-item"));
  HTMLInputElement* trackItemInput =
      HTMLInputElement::create(document(), false);
  trackItemInput->setShadowPseudoId(
      AtomicString("-internal-media-controls-text-track-list-item-input"));
  trackItemInput->setType(InputTypeNames::checkbox);
  trackItemInput->setIntegralAttribute(trackIndexAttrName(), trackIndex);
  if (!mediaElement().textTracksVisible()) {
    if (!track)
      trackItemInput->setChecked(true);
  } else {
    if (track && track->mode() == TextTrack::showingKeyword())
      trackItemInput->setChecked(true);
  }

  trackItem->appendChild(trackItemInput);
  String trackLabel = getTextTrackLabel(track);
  trackItem->appendChild(Text::create(document(), trackLabel));
  if (track && (track->label().isEmpty() || hasDuplicateLabel(track))) {
    HTMLSpanElement* trackKindMarker = HTMLSpanElement::create(document());
    if (track->kind() == track->captionsKeyword()) {
      trackKindMarker->setShadowPseudoId(AtomicString(
          "-internal-media-controls-text-track-list-kind-captions"));
    } else {
      DCHECK_EQ(track->kind(), track->subtitlesKeyword());
      trackKindMarker->setShadowPseudoId(AtomicString(
          "-internal-media-controls-text-track-list-kind-subtitles"));
    }
    trackItem->appendChild(trackKindMarker);
  }
  return trackItem;
}
