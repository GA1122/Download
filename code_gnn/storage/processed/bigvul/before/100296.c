  virtual void SaveCellularNetwork(const CellularNetwork* network) {
    DCHECK(network);
    if (!EnsureCrosLoaded() || !network)
      return;

    SetAutoConnect(network->service_path().c_str(), network->auto_connect());
  }
