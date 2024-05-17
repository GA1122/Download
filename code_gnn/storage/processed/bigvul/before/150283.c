void PasswordAutofillAgent::DidFinishDocumentLoad() {
  SendPasswordForms(false);
}
