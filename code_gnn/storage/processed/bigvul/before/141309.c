String Document::compatMode() const {
  return InQuirksMode() ? "BackCompat" : "CSS1Compat";
}
