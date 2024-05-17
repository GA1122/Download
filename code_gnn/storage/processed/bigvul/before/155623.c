base::string16 AuthenticatorTouchIdIncognitoBumpSheetModel::GetStepTitle()
    const {
#if defined(OS_MACOSX)
  return l10n_util::GetStringFUTF16(IDS_WEBAUTHN_TOUCH_ID_INCOGNITO_BUMP_TITLE,
                                    GetRelyingPartyIdString(dialog_model()));
#else
  return base::string16();
#endif   
}
