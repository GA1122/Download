  virtual ~SystemTrayDelegate() {
    AudioHandler* audiohandler = AudioHandler::GetInstance();
    if (audiohandler)
      audiohandler->RemoveVolumeObserver(this);
    DBusThreadManager::Get()->GetPowerManagerClient()->RemoveObserver(this);
    NetworkLibrary* crosnet = CrosLibrary::Get()->GetNetworkLibrary();
    if (crosnet) {
      crosnet->RemoveNetworkManagerObserver(this);
      crosnet->RemoveCellularDataPlanObserver(this);
    }
    input_method::InputMethodManager::GetInstance()->RemoveObserver(this);
    system::TimezoneSettings::GetInstance()->RemoveObserver(this);
    if (SystemKeyEventListener::GetInstance())
      SystemKeyEventListener::GetInstance()->RemoveCapsLockObserver(this);
    bluetooth_adapter_->RemoveObserver(this);
  }
