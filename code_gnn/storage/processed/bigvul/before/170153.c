void PasswordAutofillAgent::OnFindFocusedPasswordForm() {
  std::unique_ptr<PasswordForm> password_form;

  blink::WebElement element = render_frame()->GetFocusedElement();
  if (!element.isNull() && element.hasHTMLTagName("input")) {
    blink::WebInputElement input = element.to<blink::WebInputElement>();
    if (input.isPasswordField() && !input.form().isNull()) {
      if (!input.form().isNull()) {
        password_form = CreatePasswordFormFromWebForm(
            input.form(), &nonscript_modified_values_, &form_predictions_);
      } else {
        password_form = CreatePasswordFormFromUnownedInputElements(
            *render_frame()->GetWebFrame(),
            &nonscript_modified_values_, &form_predictions_);
        if (password_form->password_element != input.nameForAutofill() &&
            password_form->new_password_element != input.nameForAutofill())
          password_form.reset();
      }
    }
  }

  if (!password_form)
    password_form.reset(new PasswordForm());

  Send(new AutofillHostMsg_FocusedPasswordFormFound(
      routing_id(), *password_form));
}
