  void UpdateNetworkStatus(const char* path,
                           const char* key,
                           const Value* value) {
    if (key == NULL || value == NULL)
      return;
    if (!BrowserThread::CurrentlyOn(BrowserThread::UI)) {
      BrowserThread::PostTask(
          BrowserThread::UI, FROM_HERE,
          NewRunnableMethod(this,
                            &NetworkLibraryImpl::UpdateNetworkStatus,
                            path, key, value));
      return;
    }

    bool boolval = false;
    int intval = 0;
    std::string stringval;
    Network* network;
    if (ethernet_->service_path() == path) {
      network = ethernet_;
    } else {
      CellularNetwork* cellular =
          GetWirelessNetworkByPath(cellular_networks_, path);
      WifiNetwork* wifi =
          GetWirelessNetworkByPath(wifi_networks_, path);
      if (cellular == NULL && wifi == NULL)
        return;

      WirelessNetwork* wireless;
      if (wifi != NULL)
        wireless = static_cast<WirelessNetwork*>(wifi);
      else
        wireless = static_cast<WirelessNetwork*>(cellular);

      if (strcmp(key, kSignalStrengthProperty) == 0) {
        if (value->GetAsInteger(&intval))
          wireless->set_strength(intval);
      } else if (cellular != NULL) {
        if (strcmp(key, kRestrictedPoolProperty) == 0) {
          if (value->GetAsBoolean(&boolval))
            cellular->set_restricted_pool(boolval);
        } else if (strcmp(key, kActivationStateProperty) == 0) {
          if (value->GetAsString(&stringval))
            cellular->set_activation_state(ParseActivationState(stringval));
        } else if (strcmp(key, kPaymentURLProperty) == 0) {
          if (value->GetAsString(&stringval))
            cellular->set_payment_url(stringval);
        } else if (strcmp(key, kNetworkTechnologyProperty) == 0) {
          if (value->GetAsString(&stringval))
            cellular->set_network_technology(
                ParseNetworkTechnology(stringval));
        } else if (strcmp(key, kRoamingStateProperty) == 0) {
          if (value->GetAsString(&stringval))
            cellular->set_roaming_state(ParseRoamingState(stringval));
        }
      }
      network = wireless;
    }
    if (strcmp(key, kIsActiveProperty) == 0) {
      if (value->GetAsBoolean(&boolval))
        network->set_active(boolval);
    } else if (strcmp(key, kStateProperty) == 0) {
      if (value->GetAsString(&stringval))
        network->set_state(ParseState(stringval));
    }
    NotifyNetworkChanged(network);
  }
