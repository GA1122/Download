  void UnlockScreen() {
    mojom::SessionInfoPtr info = mojom::SessionInfo::New();
    info->state = session_manager::SessionState::ACTIVE;
    Shell::Get()->session_controller()->SetSessionInfo(std::move(info));
  }
