void HTMLMediaElement::repaint() {
  if (m_webLayer)
    m_webLayer->invalidate();

  updateDisplayState();
  if (layoutObject())
    layoutObject()->setShouldDoFullPaintInvalidation();
}
