void Gfx::opSetTextRender(Object args[], int numArgs) {
  state->setRender(args[0].getInt());
  if (args[0].getInt() == 7) {
    textHaveCSPattern = gFalse;
  }
  out->updateRender(state);
}
