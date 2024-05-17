void ArthurOutputDev::stroke(GfxState *state)
{
  m_painter->drawPath( convertPath( state, state->getPath(), Qt::OddEvenFill ) );
}
