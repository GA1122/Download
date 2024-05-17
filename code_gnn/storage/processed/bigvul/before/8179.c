void Gfx::pushResources(Dict *resDict) {
  res = new GfxResources(xref, resDict, res);
}
