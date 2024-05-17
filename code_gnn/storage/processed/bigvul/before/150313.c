bool PasswordAutofillAgent::IsUsernameOrPasswordField(
    const WebInputElement& element) {
  if (element.IsPasswordFieldForAutofill())
    return true;

  if (AutocompleteFlagForElement(element) == AutocompleteFlag::USERNAME)
    return true;

  std::unique_ptr<PasswordForm> password_form;
  if (element.Form().IsNull()) {
    DCHECK_EQ(element.GetDocument().GetFrame(), render_frame()->GetWebFrame());
    password_form = GetPasswordFormFromUnownedInputElements();
  } else {
    password_form = GetPasswordFormFromWebForm(element.Form());
  }

  if (!password_form)
    return false;
  return (password_form->username_element == element.NameForAutofill().Utf16());
}
