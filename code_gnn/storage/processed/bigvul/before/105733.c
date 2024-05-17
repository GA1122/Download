  bool IBusConnectionsAreAlive() {
    return ibus_ && ibus_bus_is_connected(ibus_) && ibus_config_;
  }
