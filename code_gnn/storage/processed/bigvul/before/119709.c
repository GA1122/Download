void ResetScreenHandler::HandleOnRestart(bool should_rollback) {
  PrefService* prefs = g_browser_process->local_state();
  prefs->SetBoolean(prefs::kFactoryResetRequested, true);
  prefs->SetBoolean(prefs::kRollbackRequested, should_rollback);
  prefs->CommitPendingWrite();

  chromeos::DBusThreadManager::Get()->GetPowerManagerClient()->RequestRestart();
}
