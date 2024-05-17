  void LockScreen() {
    mojom::SessionInfoPtr info = mojom::SessionInfo::New();
    info->state = session_manager::SessionState::LOCKED;
    Shell::Get()->session_controller()->SetSessionInfo(std::move(info));
  }
