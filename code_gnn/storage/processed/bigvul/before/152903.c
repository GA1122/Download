IntSize ImageBitmap::size() const {
  if (!m_image)
    return IntSize();
  ASSERT(m_image->width() > 0 && m_image->height() > 0);
  return IntSize(m_image->width(), m_image->height());
}
