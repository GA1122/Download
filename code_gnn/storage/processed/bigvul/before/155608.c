base::string16 AuthenticatorTransportSelectorSheetModel::GetStepTitle() const {
  return l10n_util::GetStringFUTF16(IDS_WEBAUTHN_TRANSPORT_SELECTION_TITLE,
                                    GetRelyingPartyIdString(dialog_model()));
}
