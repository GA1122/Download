TrayCast::TrayCast(SystemTray* system_tray)
    : SystemTrayItem(system_tray),
      cast_config_delegate_(ash::Shell::GetInstance()
                                ->system_tray_delegate()
                                ->GetCastConfigDelegate()) {
  Shell::GetInstance()->AddShellObserver(this);
}
