void HTMLMediaElement::networkStateChanged() {
  setNetworkState(webMediaPlayer()->getNetworkState());
}
