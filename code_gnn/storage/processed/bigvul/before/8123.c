void Gfx::opEndMarkedContent(Object args[], int numArgs) {
  if (mcStack)
    popMarkedContent();
  out->endMarkedContent(state);
}
