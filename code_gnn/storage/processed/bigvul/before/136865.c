HTMLInputElement* HTMLInputElement::Create(Document& document,
                                           const CreateElementFlags flags) {
  auto* input_element = new HTMLInputElement(document, flags);
  if (!flags.IsCreatedByParser()) {
    DCHECK(input_element->input_type_view_->NeedsShadowSubtree());
    input_element->CreateUserAgentShadowRoot();
    input_element->CreateShadowSubtree();
  }
  return input_element;
}
