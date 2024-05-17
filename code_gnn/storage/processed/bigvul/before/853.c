void ArthurOutputDev::eoClip(GfxState *state)
{
  m_painter->setClipPath(convertPath( state, state->getPath(), Qt::OddEvenFill ) );
}
