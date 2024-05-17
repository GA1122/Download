void MediaControlTextTrackListElement::setVisible(bool visible) {
  if (visible) {
    setIsWanted(true);
    refreshTextTrackListMenu();
  } else {
    setIsWanted(false);
  }
}
