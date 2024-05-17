void HTMLMediaElement::attachLayoutTree(const AttachContext& context) {
  HTMLElement::attachLayoutTree(context);

  if (layoutObject())
    layoutObject()->updateFromElement();
}
