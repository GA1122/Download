void Gfx::opBeginText(Object args[], int numArgs) {
  out->beginTextObject(state);
  drawText = gTrue;
  state->setTextMat(1, 0, 0, 1, 0, 0);
  state->textMoveTo(0, 0);
  out->updateTextMat(state);
  out->updateTextPos(state);
  fontChanged = gTrue;
  if (out->supportTextCSPattern(state)) {
    textHaveCSPattern = gTrue;
  }
}
