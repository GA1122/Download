ResetScreenHandler::~ResetScreenHandler() {
  if (delegate_)
    delegate_->OnActorDestroyed(this);
  DBusThreadManager::Get()->GetUpdateEngineClient()->RemoveObserver(this);
}
