V0CustomElementRegistrationContext* DocumentInit::RegistrationContext(
    Document* document) const {
  if (!document->IsHTMLDocument() && !document->IsXHTMLDocument())
    return nullptr;

  if (create_new_registration_context_)
    return V0CustomElementRegistrationContext::Create();

  return registration_context_.Get();
}
