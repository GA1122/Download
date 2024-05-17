void HTMLFormControlElement::AttachLayoutTree(AttachContext& context) {
  HTMLElement::AttachLayoutTree(context);

  if (!GetLayoutObject())
    return;

  GetLayoutObject()->UpdateFromElement();
}
