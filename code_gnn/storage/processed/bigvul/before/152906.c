unsigned long ImageBitmap::width() const {
  if (!m_image)
    return 0;
  ASSERT(m_image->width() > 0);
  return m_image->width();
}
