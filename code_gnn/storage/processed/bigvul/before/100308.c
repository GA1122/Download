WifiNetwork::WifiNetwork(const WifiNetwork& network)
    : WirelessNetwork(network) {
  encryption_ = network.encryption();
  passphrase_ = network.passphrase();
  identity_ = network.identity();
  cert_path_ = network.cert_path();
}
