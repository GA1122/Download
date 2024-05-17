  explicit SystemTrayDelegate(ash::SystemTray* tray)
      : tray_(tray),
        ui_weak_ptr_factory_(ALLOW_THIS_IN_INITIALIZER_LIST(
            new base::WeakPtrFactory<SystemTrayDelegate>(this))),
        network_icon_(ALLOW_THIS_IN_INITIALIZER_LIST(
                      new NetworkMenuIcon(this, NetworkMenuIcon::MENU_MODE))),
        network_icon_dark_(ALLOW_THIS_IN_INITIALIZER_LIST(
                      new NetworkMenuIcon(this, NetworkMenuIcon::MENU_MODE))),
        network_menu_(ALLOW_THIS_IN_INITIALIZER_LIST(new NetworkMenu(this))),
        clock_type_(base::k24HourClock),
        search_key_mapped_to_(input_method::kSearchKey),
        screen_locked_(false),
        state_(STATE_UNKNOWN),
        connected_network_(NULL),
        data_promo_notification_(new DataPromoNotification()) {
    AudioHandler::GetInstance()->AddVolumeObserver(this);
    DBusThreadManager::Get()->GetPowerManagerClient()->AddObserver(this);
    DBusThreadManager::Get()->GetPowerManagerClient()->RequestStatusUpdate(
        PowerManagerClient::UPDATE_INITIAL);

    NetworkLibrary* crosnet = CrosLibrary::Get()->GetNetworkLibrary();
    crosnet->AddNetworkManagerObserver(this);
    OnNetworkManagerChanged(crosnet);
    crosnet->AddCellularDataPlanObserver(this);

    input_method::InputMethodManager::GetInstance()->AddObserver(this);

    system::TimezoneSettings::GetInstance()->AddObserver(this);

    if (SystemKeyEventListener::GetInstance())
      SystemKeyEventListener::GetInstance()->AddCapsLockObserver(this);

    registrar_.Add(this,
                   chrome::NOTIFICATION_UPGRADE_RECOMMENDED,
                   content::NotificationService::AllSources());
    registrar_.Add(this,
                   chrome::NOTIFICATION_LOGIN_USER_IMAGE_CHANGED,
                   content::NotificationService::AllSources());
    if (GetUserLoginStatus() == ash::user::LOGGED_IN_NONE) {
      registrar_.Add(this,
                     chrome::NOTIFICATION_SESSION_STARTED,
                     content::NotificationService::AllSources());
    }
    registrar_.Add(this,
                   chrome::NOTIFICATION_PROFILE_CREATED,
                   content::NotificationService::AllSources());

    accessibility_enabled_.Init(prefs::kSpokenFeedbackEnabled,
                                g_browser_process->local_state(), this);

    network_icon_->SetResourceColorTheme(NetworkMenuIcon::COLOR_LIGHT);
    network_icon_dark_->SetResourceColorTheme(NetworkMenuIcon::COLOR_DARK);

    bluetooth_adapter_.reset(BluetoothAdapter::CreateDefaultAdapter());
    bluetooth_adapter_->AddObserver(this);
  }
