WifiNetwork::WifiNetwork()
    : WirelessNetwork(),
      encryption_(SECURITY_NONE) {
  type_ = TYPE_WIFI;
}
