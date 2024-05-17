void AuthenticatorClientPinEntrySheetModel::MaybeShowRetryError() {
  if (!delegate_) {
    NOTREACHED();
    return;
  }
  if (!dialog_model()->has_attempted_pin_entry()) {
    return;
  }

  base::string16 error;
  if (mode_ == AuthenticatorClientPinEntrySheetModel::Mode::kPinEntry) {
    auto attempts = dialog_model()->pin_attempts();
    error =
        attempts && *attempts <= 3
            ? l10n_util::GetPluralStringFUTF16(
                  IDS_WEBAUTHN_PIN_ENTRY_ERROR_FAILED_RETRIES, *attempts)
            : l10n_util::GetStringUTF16(IDS_WEBAUTHN_PIN_ENTRY_ERROR_FAILED);
  } else {
    DCHECK(mode_ == AuthenticatorClientPinEntrySheetModel::Mode::kPinSetup);
    error = l10n_util::GetStringUTF16(IDS_WEBAUTHN_PIN_SETUP_ERROR_FAILED);
  }
  delegate_->ShowPinError(std::move(error));
}
