void HTMLMediaElement::ReadyStateChanged() {
  SetReadyState(static_cast<ReadyState>(GetWebMediaPlayer()->GetReadyState()));
}
