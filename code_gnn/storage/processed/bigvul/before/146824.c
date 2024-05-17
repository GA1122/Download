void Document::SendSensitiveInputVisibilityInternal() {
  if (!GetFrame())
    return;

  mojom::blink::InsecureInputServicePtr insecure_input_service_ptr;
  GetFrame()->GetInterfaceProvider().GetInterface(
      mojo::MakeRequest(&insecure_input_service_ptr));
  if (password_count_ > 0) {
    insecure_input_service_ptr->PasswordFieldVisibleInInsecureContext();
    return;
  }
  insecure_input_service_ptr->AllPasswordFieldsInInsecureContextInvisible();
}
