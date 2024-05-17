bool IsOobeComplete() {
  policy::BrowserPolicyConnectorChromeOS* connector =
      g_browser_process->platform_part()->browser_policy_connector_chromeos();

  return chromeos::StartupUtils::IsOobeCompleted() &&
         (!user_manager::UserManager::Get()->GetUsers().empty() ||
          connector->IsEnterpriseManaged());
}
