void AuthenticatorBlePinEntrySheetModel::OnAccept() {
  dialog_model()->FinishPairingWithPin(pin_code_);
}
