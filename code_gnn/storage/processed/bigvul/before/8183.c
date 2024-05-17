Gfx::~Gfx() {
  while (stateGuards.size()) {
    popStateGuard();
  }
  while (state->hasSaves()) {
    error(-1, "Found state under last state guard. Popping.");
    restoreState();
  }
  if (!subPage) {
    out->endPage();
  }
  while (res) {
    popResources();
  }
  if (state) {
    delete state;
  }
  while (mcStack) {
    popMarkedContent();
  }
}
