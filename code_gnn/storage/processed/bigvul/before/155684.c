base::Optional<base::string16> PossibleResidentKeyWarning(
    AuthenticatorRequestDialogModel* dialog_model) {
  if (dialog_model->might_create_resident_credential()) {
    return l10n_util::GetStringFUTF16(IDS_WEBAUTHN_RESIDENT_KEY_PRIVACY,
                                      GetRelyingPartyIdString(dialog_model));
  }
  return base::nullopt;
}
