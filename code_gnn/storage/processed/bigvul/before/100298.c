  virtual void SaveWifiNetwork(const WifiNetwork* network) {
    DCHECK(network);
    if (!EnsureCrosLoaded() || !network)
      return;
    SetPassphrase(
        network->service_path().c_str(), network->passphrase().c_str());
    SetIdentity(network->service_path().c_str(),
        network->identity().c_str());
    SetCertPath(network->service_path().c_str(),
        network->cert_path().c_str());
    SetAutoConnect(network->service_path().c_str(), network->auto_connect());
  }
