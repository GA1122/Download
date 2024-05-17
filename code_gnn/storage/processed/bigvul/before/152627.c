void HTMLFormControlElement::attachLayoutTree(const AttachContext& context) {
  HTMLElement::attachLayoutTree(context);

  if (!layoutObject())
    return;

  layoutObject()->updateFromElement();

  if (shouldAutofocusOnAttach(this))
    document().setAutofocusElement(this);
}
