void ArthurOutputDev::updateAll(GfxState *state)
{
  OutputDev::updateAll(state);
  m_needFontUpdate = gTrue;
}
