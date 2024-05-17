WirelessNetwork::WirelessNetwork(const WirelessNetwork& network)
    : Network(network) {
  name_ = network.name();
  strength_ = network.strength();
  auto_connect_ = network.auto_connect();
  favorite_ = network.favorite();
}
