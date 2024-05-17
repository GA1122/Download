bool PasswordAutofillAgent::ShowSuggestions(const WebInputElement& element,
                                            bool show_all,
                                            bool generation_popup_showing) {
  WebInputElement username_element;
  WebInputElement password_element;
  PasswordInfo* password_info;

  if (!FindPasswordInfoForElement(element, &username_element, &password_element,
                                  &password_info)) {
    if (IsUsernameOrPasswordField(element)) {
      WebLocalFrame* frame = render_frame()->GetWebFrame();
      GURL frame_url = GURL(frame->GetDocument().Url());
#if BUILDFLAG(FULL_SAFE_BROWSING) || BUILDFLAG(SAFE_BROWSING_DB_REMOTE)
      if (!checked_safe_browsing_reputation_) {
        checked_safe_browsing_reputation_ = true;
        GURL action_url =
            element.Form().IsNull()
                ? GURL()
                : form_util::GetCanonicalActionForForm(element.Form());
        GetPasswordManagerDriver()->CheckSafeBrowsingReputation(action_url,
                                                                frame_url);
      }
#endif
    }
    return false;
  }

  if (!element.IsTextField() || !IsElementEditable(element) ||
      (!password_element.IsNull() && !IsElementEditable(password_element))) {
    return true;
  }

  if (element.Value().length() > kMaximumTextSizeForAutocomplete)
    return false;

  if (element.IsPasswordFieldForAutofill() &&
      (password_info->password_field_suggestion_was_accepted &&
       element != password_info->password_field))
    return true;

  if (generation_popup_showing)
    return false;

  return ShowSuggestionPopup(*password_info, element,
                             show_all && !element.IsPasswordFieldForAutofill(),
                             element.IsPasswordFieldForAutofill());
}
