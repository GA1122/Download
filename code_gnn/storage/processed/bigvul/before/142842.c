void HTMLMediaElement::NetworkStateChanged() {
  SetNetworkState(GetWebMediaPlayer()->GetNetworkState());
}
