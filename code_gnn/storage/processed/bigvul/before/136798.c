CustomElementRegistry* LocalDOMWindow::customElements() const {
  if (!custom_elements_ && document_)
    custom_elements_ = CustomElementRegistry::Create(this);
  return custom_elements_;
}
