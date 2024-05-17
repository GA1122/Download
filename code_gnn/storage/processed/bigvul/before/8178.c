void Gfx::pushMarkedContent() {
  MarkedContentStack *mc = new MarkedContentStack();
  mc->ocSuppressed = gFalse;
  mc->next = mcStack;
  mcStack = mc;
}
