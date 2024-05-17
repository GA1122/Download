network::mojom::ConnectionType GetConnectionType(
    network::NetworkConnectionTracker* network_connection_tracker) {
  auto connection_type = network::mojom::ConnectionType::CONNECTION_UNKNOWN;
  network_connection_tracker->GetConnectionType(&connection_type,
                                                base::DoNothing());
  return connection_type;
}
