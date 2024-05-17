AXObject* AXObjectCacheImpl::createFromInlineTextBox(
    AbstractInlineTextBox* inlineTextBox) {
  return AXInlineTextBox::create(inlineTextBox, *this);
}
