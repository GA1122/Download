void ResetScreenHandler::HandleOnCancel() {
  if (delegate_)
    delegate_->OnExit();
  DBusThreadManager::Get()->GetUpdateEngineClient()->RemoveObserver(this);
}
