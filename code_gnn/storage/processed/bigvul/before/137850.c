void MediaControlToggleClosedCaptionsButtonElement::defaultEventHandler(
    Event* event) {
  if (event->type() == EventTypeNames::click) {
    if (mediaElement().textTracks()->length() == 1) {
      if (mediaElement().textTracks()->hasShowingTracks()) {
        mediaControls().disableShowingTextTracks();
      } else {
        mediaControls().showTextTrackAtIndex(0);
      }
    } else {
      mediaControls().toggleTextTrackList();
    }

    updateDisplayType();
    event->setDefaultHandled();
  }

  MediaControlInputElement::defaultEventHandler(event);
}
