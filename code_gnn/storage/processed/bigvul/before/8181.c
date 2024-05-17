void Gfx::restoreState() {
  if (stackHeight <= bottomGuard() || !state->hasSaves()) {
    error(-1, "Restoring state when no valid states to pop");
    commandAborted = gTrue;
    return;
  }
  state = state->restore();
  out->restoreState(state);
  stackHeight--;
}
