void HTMLMediaElement::readyStateChanged() {
  setReadyState(static_cast<ReadyState>(webMediaPlayer()->getReadyState()));
}
