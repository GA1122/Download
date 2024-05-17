void AuthenticatorClientPinEntrySheetModel::OnAccept() {
  constexpr size_t kMinPinLength = 4;
  if (!delegate_) {
    NOTREACHED();
    return;
  }
  if (mode_ == AuthenticatorClientPinEntrySheetModel::Mode::kPinSetup) {
    base::Optional<base::string16> error;
    if (!pin_code_.empty() && !IsValidUTF16(pin_code_)) {
      error = l10n_util::GetStringUTF16(
          IDS_WEBAUTHN_PIN_ENTRY_ERROR_INVALID_CHARACTERS);
    } else if (pin_code_.size() < kMinPinLength) {
      error = l10n_util::GetStringUTF16(IDS_WEBAUTHN_PIN_ENTRY_ERROR_TOO_SHORT);
    } else if (pin_code_ != pin_confirmation_) {
      error = l10n_util::GetStringUTF16(IDS_WEBAUTHN_PIN_ENTRY_ERROR_MISMATCH);
    }
    if (error) {
      delegate_->ShowPinError(*error);
      return;
    }
  } else {
    DCHECK(mode_ == AuthenticatorClientPinEntrySheetModel::Mode::kPinEntry);
    if (pin_code_.size() < kMinPinLength) {
      delegate_->ShowPinError(
          l10n_util::GetStringUTF16(IDS_WEBAUTHN_PIN_ENTRY_ERROR_TOO_SHORT));
      return;
    }
  }

  if (dialog_model()) {
    dialog_model()->OnHavePIN(base::UTF16ToUTF8(pin_code_));
  }
}
