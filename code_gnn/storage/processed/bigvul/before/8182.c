void Gfx::saveState() {
  out->saveState(state);
  state = state->save();
  stackHeight++;
}
