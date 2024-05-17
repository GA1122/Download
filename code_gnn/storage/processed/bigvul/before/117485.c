void TestingAutomationProvider::RemoveChromeosObservers() {
  chromeos::DBusThreadManager::Get()->GetPowerManagerClient()->
      RemoveObserver(power_manager_observer_);
  delete power_manager_observer_;
  power_manager_observer_ = NULL;
}
