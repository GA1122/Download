void TestingAutomationProvider::AddChromeosObservers() {
  power_manager_observer_ = new PowerManagerClientObserverForTesting;
  chromeos::DBusThreadManager::Get()->GetPowerManagerClient()->
      AddObserver(power_manager_observer_);
}
