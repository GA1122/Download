void ShelfWidget::Initialize() {
  OnSessionStateChanged(Shell::Get()->session_controller()->GetSessionState());
}
