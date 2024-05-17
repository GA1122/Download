void HTMLMediaElement::AttachLayoutTree(AttachContext& context) {
  HTMLElement::AttachLayoutTree(context);

  if (GetLayoutObject())
    GetLayoutObject()->UpdateFromElement();
}
