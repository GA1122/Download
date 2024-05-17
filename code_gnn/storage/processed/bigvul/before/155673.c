void AuthenticatorBlePairingBeginSheetModel::OnAccept() {
  dialog_model()->SetCurrentStep(
      AuthenticatorRequestDialogModel::Step::kBleDeviceSelection);
}
