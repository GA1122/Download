unsigned long ImageBitmap::height() const {
  if (!m_image)
    return 0;
  ASSERT(m_image->height() > 0);
  return m_image->height();
}
