bool ScrollAnchor::ComputeScrollAnchorDisablingStyleChanged() {
  LayoutObject* current = AnchorObject();
  if (!current)
    return false;

  LayoutObject* scroller_box = ScrollerLayoutBox(scroller_);
  while (true) {
    DCHECK(current);
    if (current->ScrollAnchorDisablingStyleChanged())
      return true;
    if (current == scroller_box)
      return false;
    current = current->Parent();
  }
}
