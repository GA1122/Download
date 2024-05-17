bool PasswordAutofillAgent::ShowSuggestionPopup(
    const PasswordInfo& password_info,
    const WebInputElement& user_input,
    bool show_all,
    bool show_on_password_field) {
  DCHECK(!user_input.IsNull());
  WebFrame* frame = user_input.GetDocument().GetFrame();
  if (!frame)
    return false;

  WebView* webview = frame->View();
  if (!webview)
    return false;

  if (user_input.IsPasswordFieldForAutofill() && !user_input.IsAutofilled() &&
      !user_input.Value().IsEmpty()) {
    HidePopup();
    return false;
  }

  FormData form;
  FormFieldData field;
  form_util::FindFormAndFieldForFormControlElement(user_input, &form, &field);

  int options = 0;
  if (show_all)
    options |= SHOW_ALL;
  if (show_on_password_field)
    options |= IS_PASSWORD_FIELD;

  base::string16 username_string(user_input.IsPasswordFieldForAutofill()
                                     ? base::string16()
                                     : user_input.Value().Utf16());

  GetPasswordManagerDriver()->ShowPasswordSuggestions(
      field.text_direction, username_string, options,
      render_frame()->GetRenderView()->ElementBoundsInWindow(user_input));
  username_query_prefix_ = username_string;
  return CanShowSuggestion(password_info.fill_data, username_string, show_all);
}
