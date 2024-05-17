  ~DBusServices() {
    CertLibrary::Shutdown();
    NetworkHandler::Shutdown();
    cryptohome::AsyncMethodCaller::Shutdown();
    disks::DiskMountManager::Shutdown();
    LoginState::Shutdown();
    CertLoader::Shutdown();
    TPMTokenLoader::Shutdown();
    proxy_resolution_service_.reset();
    kiosk_info_service_.reset();
    liveness_service_.reset();
    virtual_file_request_service_.reset();
    component_updater_service_.reset();
    finch_features_service_.reset();
    vm_applications_service_.reset();
    drive_file_stream_service_.reset();
    PowerDataCollector::Shutdown();
    if (GetAshConfig() != ash::Config::MASH)
      PowerPolicyController::Shutdown();
    device::BluetoothAdapterFactory::Shutdown();
    bluez::BluezDBusManager::Shutdown();
  }
