void ArthurOutputDev::startPage(int pageNum, GfxState *state)
{
  int w = static_cast<int>(state->getPageWidth());
  int h = static_cast<int>(state->getPageHeight());
  QColor fillColour(Qt::white);
  QBrush fill(fillColour);
  m_painter->save();
  m_painter->setPen(fillColour);
  m_painter->setBrush(fill);
  m_painter->drawRect(0, 0, w, h);
  m_painter->restore();
}
