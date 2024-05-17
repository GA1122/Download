void Gfx::popMarkedContent() {
  MarkedContentStack *mc = mcStack;
  mcStack = mc->next;
  delete mc;
}
