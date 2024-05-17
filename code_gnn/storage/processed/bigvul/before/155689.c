void AuthenticatorClientPinEntrySheetModel::SetPinConfirmation(
    base::string16 pin_confirmation) {
  DCHECK(mode_ == AuthenticatorClientPinEntrySheetModel::Mode::kPinSetup);
  pin_confirmation_ = std::move(pin_confirmation);
}
