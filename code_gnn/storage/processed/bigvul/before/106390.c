bool OfflineLoadPage::ShowActivationMessage() {
  CrosLibrary* cros = CrosLibrary::Get();
  if (!cros || !cros->GetNetworkLibrary()->cellular_available())
    return false;

  const CellularNetworkVector& cell_networks =
      cros->GetNetworkLibrary()->cellular_networks();
  for (size_t i = 0; i < cell_networks.size(); ++i) {
    chromeos::ActivationState activation_state =
        cell_networks[i]->activation_state();
    if (activation_state == ACTIVATION_STATE_ACTIVATED)
      return false;
  }
  return true;
}
