void AnnotateFormsAndFieldsWithSignatures(WebLocalFrame* frame,
                                          WebVector<WebFormElement>* forms) {
  for (WebFormElement& form : *forms) {
    std::unique_ptr<PasswordForm> password_form(
        CreateSimplifiedPasswordFormFromWebForm(
            form,  nullptr,
             nullptr));
    WebString form_signature;
    if (password_form) {
      form_signature = GetFormSignatureAsWebString(*password_form);
      form.SetAttribute(WebString::FromASCII(kDebugAttributeForFormSignature),
                        form_signature);
    }
    std::vector<WebFormControlElement> form_fields =
        form_util::ExtractAutofillableElementsInForm(form);
    AnnotateFieldsWithSignatures(&form_fields, form_signature);
  }

  std::vector<WebFormControlElement> unowned_elements =
      form_util::GetUnownedAutofillableFormFieldElements(
          frame->GetDocument().All(), nullptr);
  std::unique_ptr<PasswordForm> password_form(
      CreateSimplifiedPasswordFormFromUnownedInputElements(
          *frame,  nullptr,
           nullptr));
  WebString form_signature;
  if (password_form)
    form_signature = GetFormSignatureAsWebString(*password_form);
  AnnotateFieldsWithSignatures(&unowned_elements, form_signature);
}
