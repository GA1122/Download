  explicit DBusServices(const content::MainFunctionParams& parameters) {
    bluez::BluezDBusManager::Initialize(
        DBusThreadManager::Get()->GetSystemBus(),
        chromeos::DBusThreadManager::Get()->IsUsingFakes());

    if (GetAshConfig() != ash::Config::MASH) {
      PowerPolicyController::Initialize(
          DBusThreadManager::Get()->GetPowerManagerClient());
    }

    proxy_resolution_service_ = CrosDBusService::Create(
        kNetworkProxyServiceName, dbus::ObjectPath(kNetworkProxyServicePath),
        CrosDBusService::CreateServiceProviderList(
            std::make_unique<ProxyResolutionServiceProvider>(
                std::make_unique<
                    ChromeProxyResolutionServiceProviderDelegate>())));

    kiosk_info_service_ = CrosDBusService::Create(
        kKioskAppServiceName, dbus::ObjectPath(kKioskAppServicePath),
        CrosDBusService::CreateServiceProviderList(
            std::make_unique<KioskInfoService>()));

    liveness_service_ = CrosDBusService::Create(
        kLivenessServiceName, dbus::ObjectPath(kLivenessServicePath),
        CrosDBusService::CreateServiceProviderList(
            std::make_unique<LivenessServiceProvider>()));

    screen_lock_service_ = CrosDBusService::Create(
        kScreenLockServiceName, dbus::ObjectPath(kScreenLockServicePath),
        CrosDBusService::CreateServiceProviderList(
            std::make_unique<ScreenLockServiceProvider>()));

    virtual_file_request_service_ = CrosDBusService::Create(
        kVirtualFileRequestServiceName,
        dbus::ObjectPath(kVirtualFileRequestServicePath),
        CrosDBusService::CreateServiceProviderList(
            std::make_unique<VirtualFileRequestServiceProvider>(
                std::make_unique<
                    ChromeVirtualFileRequestServiceProviderDelegate>())));

    component_updater_service_ = CrosDBusService::Create(
        kComponentUpdaterServiceName,
        dbus::ObjectPath(kComponentUpdaterServicePath),
        CrosDBusService::CreateServiceProviderList(
            std::make_unique<ComponentUpdaterServiceProvider>(
                g_browser_process->platform_part()->cros_component_manager())));

    finch_features_service_ = CrosDBusService::Create(
        kChromeFeaturesServiceName,
        dbus::ObjectPath(kChromeFeaturesServicePath),
        CrosDBusService::CreateServiceProviderList(
            std::make_unique<ChromeFeaturesServiceProvider>(
                std::make_unique<FinchFeaturesServiceProviderDelegate>())));

    vm_applications_service_ = CrosDBusService::Create(
        vm_tools::apps::kVmApplicationsServiceName,
        dbus::ObjectPath(vm_tools::apps::kVmApplicationsServicePath),
        CrosDBusService::CreateServiceProviderList(
            std::make_unique<VmApplicationsServiceProvider>(
                std::make_unique<VmApplicationsServiceProviderDelegate>())));

    drive_file_stream_service_ = CrosDBusService::Create(
        drivefs::kDriveFileStreamServiceName,
        dbus::ObjectPath(drivefs::kDriveFileStreamServicePath),
        CrosDBusService::CreateServiceProviderList(
            std::make_unique<DriveFileStreamServiceProvider>(
                std::make_unique<
                    drivefs::DriveFileStreamServiceProviderDelegate>())));

    PowerDataCollector::Initialize();

    LoginState::Initialize();
    TPMTokenLoader::Initialize();
    CertLoader::Initialize();

    disks::DiskMountManager::Initialize();
    cryptohome::AsyncMethodCaller::Initialize();
    cryptohome::HomedirMethods::Initialize();

    NetworkHandler::Initialize();
    CertLibrary::Initialize();

    NetworkChangeNotifierFactoryChromeos::GetInstance()->Initialize();

    UpgradeDetectorChromeos::GetInstance()->Init();

    DeviceSettingsService::Get()->SetSessionManager(
        DBusThreadManager::Get()->GetSessionManagerClient(),
        OwnerSettingsServiceChromeOSFactory::GetInstance()->GetOwnerKeyUtil());
  }
