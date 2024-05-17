void HTMLMediaElement::scheduleNotifyPlaying() {
  scheduleEvent(EventTypeNames::playing);
  scheduleResolvePlayPromises();
}
