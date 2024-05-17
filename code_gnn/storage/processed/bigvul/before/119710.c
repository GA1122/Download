void ResetScreenHandler::Hide() {
  DBusThreadManager::Get()->GetUpdateEngineClient()->RemoveObserver(this);
}
