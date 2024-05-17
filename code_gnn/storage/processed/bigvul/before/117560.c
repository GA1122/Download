void SystemKeyEventListener::OnBrightnessDown() {
  DBusThreadManager::Get()->GetPowerManagerClient()->
      DecreaseScreenBrightness(true);
}
