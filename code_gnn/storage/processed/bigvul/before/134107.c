chromeos::IBusEngineHandlerInterface* GetEngine() {
  return chromeos::IBusBridge::Get()->GetEngineHandler();
}
