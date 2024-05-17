 void HTMLMediaElement::setNetworkState(NetworkState state) {
  if (m_networkState == state)
    return;

  m_networkState = state;
  if (mediaControls())
    mediaControls()->networkStateChanged();
}
