base::string16 AuthenticatorClientPinEntrySheetModel::GetStepDescription()
    const {
  return l10n_util::GetStringUTF16(
      mode_ == AuthenticatorClientPinEntrySheetModel::Mode::kPinEntry
          ? IDS_WEBAUTHN_PIN_ENTRY_DESCRIPTION
          : IDS_WEBAUTHN_PIN_SETUP_DESCRIPTION);
}
