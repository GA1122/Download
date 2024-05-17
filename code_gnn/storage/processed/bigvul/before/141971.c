void AutofillExternalDelegate::ApplyAutofillOptions(
    std::vector<Suggestion>* suggestions,
    bool is_all_server_suggestions) {
  if (query_field_.is_autofilled) {
    base::string16 value =
        l10n_util::GetStringUTF16(IDS_AUTOFILL_CLEAR_FORM_MENU_ITEM);
#if defined(OS_ANDROID)
    if (IsKeyboardAccessoryEnabled())
      value = base::i18n::ToUpper(value);
#endif

    suggestions->push_back(Suggestion(value));
    suggestions->back().frontend_id = POPUP_ITEM_ID_CLEAR_FORM;
  }

  suggestions->push_back(Suggestion(GetSettingsSuggestionValue()));
  suggestions->back().frontend_id = POPUP_ITEM_ID_AUTOFILL_OPTIONS;
  if (is_all_server_suggestions)
    suggestions->back().icon = base::ASCIIToUTF16("googlePay");

#if defined(OS_IOS)
  if (base::FeatureList::IsEnabled(
          features::kAutofillDownstreamUseGooglePayBrandingOniOS) &&
      is_all_server_suggestions) {
    Suggestion googlepay_icon;
    googlepay_icon.icon = base::ASCIIToUTF16("googlePay");
    googlepay_icon.frontend_id = POPUP_ITEM_ID_GOOGLE_PAY_BRANDING;
    suggestions->insert(suggestions->begin(), googlepay_icon);
  }
#endif

#if defined(OS_ANDROID)
  if (IsKeyboardAccessoryEnabled()) {
    suggestions->back().icon = base::ASCIIToUTF16("settings");
    if (IsHintEnabledInKeyboardAccessory() && !query_field_.is_autofilled) {
      Suggestion create_icon;
      create_icon.icon = base::ASCIIToUTF16("create");
      create_icon.frontend_id = POPUP_ITEM_ID_CREATE_HINT;
      suggestions->push_back(create_icon);
    }
  }
#endif
}
