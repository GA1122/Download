void AuthenticatorClientPinEntrySheetModel::SetPinCode(
    base::string16 pin_code) {
  pin_code_ = std::move(pin_code);
}
