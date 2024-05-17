GBool Gfx::contentIsHidden() {
  MarkedContentStack *mc = mcStack;
  bool hidden = mc && mc->ocSuppressed;
  while (!hidden && mc && mc->next) {
    mc = mc->next;
    hidden = mc->ocSuppressed;
  }
  return hidden;
}
