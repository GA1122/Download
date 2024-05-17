bool ShelfWidget::IsUsingViewsShelf() {
  switch (Shell::Get()->session_controller()->GetSessionState()) {
    case session_manager::SessionState::ACTIVE:
      return true;
    case session_manager::SessionState::OOBE:
    case session_manager::SessionState::LOGIN_PRIMARY:
      return true;
    case session_manager::SessionState::LOCKED:
    case session_manager::SessionState::LOGIN_SECONDARY:
      return switches::IsUsingViewsLock();
    case session_manager::SessionState::UNKNOWN:
    case session_manager::SessionState::LOGGED_IN_NOT_ACTIVE:
      return features::IsViewsLoginEnabled();
  }
}
