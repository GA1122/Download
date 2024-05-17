bool HTMLMediaElement::couldPlayIfEnoughData() const {
  return !paused() && !endedPlayback() && !stoppedDueToErrors();
}
